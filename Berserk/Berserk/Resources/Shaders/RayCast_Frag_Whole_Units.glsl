#version 130

#define MARCH_STEP_SIZE 0.01
#define MARCH_MAX_NUM_STEPS 1000

#define TILE_WALL_COLOR vec3(1.0f)
#define TILE_GOAL_COLOR vec3(0.0f, 1.0f, 0.0f)

// Constants
const float PI = atan(-1.0f);
const float FOV = PI * 0.9f;
const int MAX_RENDER_ENTITIES = 64;

// Uniforms
uniform sampler2D u_mapTexture;
uniform sampler2D u_wallTexture;
uniform sampler2D u_floorTexture;
uniform sampler2D u_goalTexture;
uniform sampler2D u_entityTexture;

uniform vec4 u_entityTexRects[MAX_RENDER_ENTITIES];

uniform vec3 u_cameraPosition;	// vec3(x, y, z)
uniform vec3 u_entityPositions[MAX_RENDER_ENTITIES];
uniform vec3 u_fogColor;

uniform vec2 u_cameraRotation; // vec2(yaw, roll)
uniform vec2 u_resolution;
uniform vec2 u_entityWorldScales[MAX_RENDER_ENTITIES];

uniform float u_timer;

uniform int u_numEntities;

vec2 MAP_SIZE = textureSize(u_mapTexture, 0);
vec2 ONE_OVER_MAP_SIZE = 1.0f / MAP_SIZE;
vec2 ONE_OVER_ENTITY_TEXTURE_SIZE = 1.0f / textureSize(u_entityTexture, 0);

mat2x2 rotate(float a)
{
	float c = cos(a);
	float s = sin(a);

	return mat2x2(c, s, -s, c);
}

void main()
{
	// Make uvs go from -0.5 to 0.5 (with aspect correction)
	vec2 uv = gl_FragCoord.xy / u_resolution;
	uv -= 0.5;

	float aspectRatio = u_resolution.x / u_resolution.y;
	uv.x *= aspectRatio;

	// Rotate uvs
	uv *= rotate(u_cameraRotation.y);
	
	// Keep track of camera in the world
	//vec2 camPos = u_cameraPosition.xy * ONE_OVER_MAP_SIZE;
	vec2 camPos = u_cameraPosition.xy;

	// Create ray
	//float dist = 0.001f;
	float dist = 0.0f;
	float screenAngle = uv.x * FOV;
	float angle = -(u_cameraRotation.x + screenAngle);	// Flip since y is inverted
	vec2 rayDir = vec2(cos(angle), sin(angle));
	vec2 rayPos = camPos + rayDir * dist;

	// March
	bool foundWall = false;
	bool wallIsGoal = false;
	for(int i = 0; i < MARCH_MAX_NUM_STEPS && !foundWall; i++)
	{
		dist += MARCH_STEP_SIZE;
		rayPos += rayDir * MARCH_STEP_SIZE;

		// Sample
		//vec3 foundCol = texture2D(u_mapTexture, rayPos * ONE_OVER_MAP_SIZE).rgb;
		vec3 foundCol = texelFetch(u_mapTexture, ivec2(rayPos), 0).rgb;
		vec3 diff = foundCol - TILE_WALL_COLOR;

		// Hit wall
		if(dot(diff, diff) <= 0.1f)
			foundWall = true;

		// Hit goal wall
		diff = foundCol - TILE_GOAL_COLOR;
		if(dot(diff, diff) <= 0.1f)
		{
			foundWall = true;
			wallIsGoal = true;
		}
	}

	// Fog and wall height
	float oneOverDist = 1.0f / dist;
	float fog = clamp(1.0f - (dist / (MARCH_MAX_NUM_STEPS * MARCH_STEP_SIZE)), 0.0, 1.0);
	float halfWallHeight = oneOverDist;
	float wallCameraHeightY = uv.y + (u_cameraPosition.z * 0.5f) * oneOverDist * 0.1f; // Where did this 0.1 come from? D:
	float wall = abs(wallCameraHeightY) < halfWallHeight ? 1.0f : 0.0f;

	// Find wall uvs
	vec2 hitPos = fract(rayPos);
	vec2 diff = hitPos - 0.5f;
	float isNormalCloseToPositiveDiagonal = dot(diff, vec2(1)) < 0.0f ? 1.0 : 0.0;

	// Choose correct U-value based on if the normal is close to the positive diagonal or not
	float wallU = mix(min(hitPos.x, hitPos.y), max(hitPos.x, hitPos.y), isNormalCloseToPositiveDiagonal);
	float wallV = (uv.y / halfWallHeight) * 0.5f + 0.5f + u_cameraPosition.z * 0.5f;
	vec2 wallUV = vec2(wallU, 1.0f - wallV);

	// Sample wall texture
	vec3 wallCol = texture2D(
		u_wallTexture, 
		wallUV * vec2(0.5f, 1.0f) + vec2(floor(fract(u_timer) * 2.0f), 0.0f) * 0.5f
	).rgb;

	// Sample goal texture
	if(wallIsGoal)
	{
		// Flip u-coordinate to animate
		float tempWallU = fract(u_timer * 2.0f) < 0.5f ? 1.0f - wallUV.x : wallUV.x;

		vec4 goalWallCol = texture2D(
			u_goalTexture, 
			vec2(
				tempWallU,
				wallUV.y
			)
		);

		wallCol = mix(wallCol, goalWallCol.rgb, goalWallCol.a);
	}

	// Sample floor texture
	float floorDist = (0.80f / -uv.y);
	float floorFog = clamp(mix(MARCH_MAX_NUM_STEPS * MARCH_STEP_SIZE, 0.0, floorDist * 0.1f * 0.5f), 0.0, 1.0);
	vec3 floorCol = texture2D(
		u_floorTexture,
		fract(camPos + rayDir * floorDist * (u_cameraPosition.z + 1.0f) * 0.65f)	//fract((camPos * MAP_SIZE + rayDir * floorDist * (u_cameraPosition.z + 1.0f)))
	).rgb;


	// Wall/floor is visible
	float showWall = pow(wall * fog, 1.0f);
	float showFloor = (1.0 - wall) * floorFog;

	// Wall + floor
	vec3 col = (wallCol * showWall) + (floorCol * showFloor);
	col = mix(
		u_fogColor * (uv.y > 0.0f ? max(1.0f - uv.y, wall) : 1.0f), 
		col, 
		(showWall + showFloor) * (wallCameraHeightY > halfWallHeight ? 0.0f : 1.0f)
	);

	// Render entities
	//float currentPixelDepth = length((rayDir * dist) * MAP_SIZE);
	float currentPixelDepth = length(rayDir * dist);
	for(int i = 0; i < u_numEntities; i++)
	{
		// Convert position to normalized screen coordinates
		vec2 camToTarget = vec2(
			u_entityPositions[i].x - u_cameraPosition.x, 
			u_entityPositions[i].y - u_cameraPosition.y
		);
		float spriteDist = length(camToTarget);

		vec2 camToTargetDir = camToTarget / spriteDist;
		vec2 dir = vec2(cos(u_cameraRotation.x), -sin(u_cameraRotation.x));

		float negateAngle = cross(vec3(camToTarget, 0.0f), vec3(dir, 0.0f)).z < 0.0f ? -1.0f : 1.0f;
		
		// A minimum cosine value of 1 is given, since the value could be undefined if the dot
		// product between the two unit vectors are above 1, because of precision errors
		float deltaAngle = acos(min(dot(dir, camToTargetDir), 1.0f)) * negateAngle;

		float oneOverSpriteDist = 1.0f / spriteDist;

		float screenPosX = deltaAngle / FOV;	// deltaAngle / (FOV * 0.5f) * 0.5f
		float screenPosY = (u_entityPositions[i].z - u_cameraPosition.z) * oneOverSpriteDist;

		// Sprite size
		vec2 spriteSize = u_entityWorldScales[i] * oneOverSpriteDist;

		// Is pixel close enough to the sprite?
		if(abs(uv.x - screenPosX) <= spriteSize.x && 
			abs(uv.y - screenPosY) <= spriteSize.y && spriteDist < currentPixelDepth)
		{
			// Sprite texture coordinates
			vec2 spriteUV = vec2(
				(uv.x - screenPosX) / spriteSize.x * 0.5f + 0.5f,
				1.0f - ((uv.y - screenPosY) / spriteSize.y * 0.5f + 0.5f)
			);

			// Texture region
			spriteUV.xy *= u_entityTexRects[i].zw * ONE_OVER_ENTITY_TEXTURE_SIZE;
			spriteUV.xy += u_entityTexRects[i].xy * ONE_OVER_ENTITY_TEXTURE_SIZE;

			vec4 spriteCol = texture2D(u_entityTexture, spriteUV);

			// If pixel is not transparent
			if(spriteCol.a > 0.0f)
			{
				// Update depth
				currentPixelDepth = spriteDist;

				// Apply color
				col = spriteCol.rgb;
			}
		}	
	}

	gl_FragColor = vec4(col, 1.0);
}
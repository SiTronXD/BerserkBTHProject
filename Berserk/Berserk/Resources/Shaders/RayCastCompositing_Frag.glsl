#version 130

#define EPSILON 0.000001

// Constants
const float PI = atan(-1.0);
const float FOV = PI * 0.6;
const int MAX_RENDER_ENTITIES = 128;

// Uniforms
uniform sampler2D u_rayCastTexture;
uniform sampler2D u_mapTexture;
uniform sampler2D u_wallTexture;
uniform sampler2D u_floorTexture;
uniform sampler2D u_goalTexture;
uniform sampler2D u_entityTexture;
uniform sampler2D u_floorEffectsTexture;

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
vec2 ONE_OVER_MAP_SIZE = 1.0 / MAP_SIZE;
vec2 ONE_OVER_ENTITY_TEXTURE_SIZE = 1.0 / textureSize(u_entityTexture, 0);

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
	
	// Zoom in to avoid artifacts from sampling outside u_rayCastTexture 
	// at the corners of the window :p
	uv /= abs(u_cameraRotation.y) * 0.3 + 1.0;
	
	vec2 rayHitInfoUV = vec2(uv.x, 0.0) / vec2(aspectRatio, 1.0) + vec2(0.5, 0.0);
	vec4 rayHitInfo = texture2D(u_rayCastTexture, rayHitInfoUV);

	// Keep track of camera in the world
	vec2 camPos = u_cameraPosition.xy * ONE_OVER_MAP_SIZE;

	// Create ray
	float decompressedDist = (rayHitInfo.z * 65280.0 + rayHitInfo.w * 255.0) / (65535.0);
	float dist = decompressedDist;
	float screenAngle = uv.x * FOV;
	float angle = -(u_cameraRotation.x + screenAngle);	// Flip since y is inverted
	vec2 rayDir = vec2(cos(angle), sin(angle));

	// March
	bool wallIsGoal = rayHitInfo.y <= 0.0;
	float distMaxLength = 16.0 * ONE_OVER_MAP_SIZE.x;
	
	// Fog and wall height
	float oneOverDist = 1.0 / dist;
	float fog = clamp(1.0 - (dist / distMaxLength), 0.0, 1.0);
	float halfWallHeight = ONE_OVER_MAP_SIZE.x * oneOverDist;
	float wallCameraHeightY = uv.y + u_cameraPosition.z * oneOverDist * ONE_OVER_MAP_SIZE.x;
	float wall = abs(wallCameraHeightY) < halfWallHeight ? 1.0 : 0.0;

	// Choose correct U-value based on if the normal is close to the positive diagonal or not
	float wallU = rayHitInfo.x;
	float wallV = (uv.y / halfWallHeight) * 0.5 + 0.5 + u_cameraPosition.z * 0.5;
	vec2 wallUV = vec2(wallU, 1.0 - wallV);

	// Sample wall texture, multiplied with wall type color
	float wallTypeU = wallIsGoal ? 3.5 / 8.0 : rayHitInfo.y;
	float xOffset = floor(fract(u_timer) * 2.0) * 0.5; 
	vec2 animatedWallUV = wallUV * vec2(0.49, 1.0) + vec2(xOffset, 0.0);	// 0.49 since 0.5 gave bad precision in certain map corners

	vec3 wallCol = texture2D(
		u_wallTexture, 
		animatedWallUV
	).rgb;

	// Sample goal texture
	if(wallIsGoal)
	{
		// Flip u-coordinate to animate
		float tempWallU = fract(u_timer * 2.0) < 0.5 ? 1.0 - wallUV.x : wallUV.x;

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
	float floorDist = (0.8 / -uv.y);
	float floorFog = clamp(1.0 - ((floorDist * 1.25 * ONE_OVER_MAP_SIZE.x) / distMaxLength), 0.0, 1.0);
	vec2 floorUV = (camPos * MAP_SIZE  + rayDir * floorDist / 0.8 * (u_cameraPosition.z + 1.0));
	vec3 floorCol = texture2D(
		u_floorTexture,
		fract(floorUV)
	).rgb;

	// Add effects on top of floor
	vec4 floorEffectsCol = texture2D(u_floorEffectsTexture, floorUV * ONE_OVER_MAP_SIZE);
	floorCol = mix(floorCol, floorEffectsCol.rgb, floorEffectsCol.a);


	// Wall/floor is visible
	float showWall = pow(wall * fog, 0.2);
	float showFloor = (1.0 - wall) * floorFog;

	// Wall + floor
	vec3 col = (wallCol * showWall) + (floorCol * showFloor);
	col = mix(
		u_fogColor * (uv.y > 0.0 ? max(1.0 - (uv.y-0.05), wall) : 1.0), 
		col, 
		(showWall + showFloor) * (wallCameraHeightY > halfWallHeight ? 0.0 : 1.0)
	);

	// Render entities
	float currentPixelDepth = length(rayDir * dist * MAP_SIZE);
	vec2 dir = vec2(cos(u_cameraRotation.x), -sin(u_cameraRotation.x));
	for(int i = 0; i < u_numEntities; i++)
	{
		// Convert position to normalized screen coordinates
		vec2 camToTarget = vec2(
			u_entityPositions[i].x - u_cameraPosition.x, 
			u_entityPositions[i].y - u_cameraPosition.y
		);
		float spriteDist = length(camToTarget);

		// Check if the sprite is infront of the closest wall
		if(spriteDist < currentPixelDepth)
		{
			vec2 camToTargetDir = camToTarget / spriteDist;

			float negateAngle = cross(vec3(camToTarget, 0.0), vec3(dir, 0.0)).z < 0.0 ? -1.0 : 1.0;
		
			// The cosine value is clamped, since the value could be undefined if the dot
			// product between the two unit vectors are outside the range [-1, 1], because of precision errors
			float deltaAngle = acos(clamp(dot(dir, camToTargetDir), -1.0, 1.0)) * negateAngle;

			float oneOverSpriteDist = 1.0 / max(spriteDist, 0.001);

			float screenPosX = deltaAngle / FOV;	// deltaAngle / (FOV * 0.5f) * 0.5f
			float screenPosY = (u_entityPositions[i].z - u_cameraPosition.z) * oneOverSpriteDist;

			// Sprite size
			vec2 spriteSize = u_entityWorldScales[i] * oneOverSpriteDist;

			// Is pixel close enough to the sprite?
			if(abs(uv.x - screenPosX) <= spriteSize.x && 
				abs(uv.y - screenPosY) <= spriteSize.y)
			{
				// Sprite texture coordinates
				vec2 spriteUV = vec2(
					(uv.x - screenPosX) / spriteSize.x * 0.5 + 0.5,
					1.0 - ((uv.y - screenPosY) / spriteSize.y * 0.5 + 0.5)
				);

				// Texture region
				vec2 spriteRegionSize = u_entityTexRects[i].zw * ONE_OVER_ENTITY_TEXTURE_SIZE;
				vec2 spriteRegionTranslation = u_entityTexRects[i].xy * ONE_OVER_ENTITY_TEXTURE_SIZE;
				spriteUV.xy *= spriteRegionSize;
				spriteUV.xy += spriteRegionTranslation;

				// Clamp coordinates within region to avoid precision glitches
				spriteUV.xy = clamp(
					spriteUV.xy, 
					spriteRegionTranslation - spriteRegionSize * min(vec2(0), sign(spriteRegionSize)) + vec2(EPSILON), 
					spriteRegionTranslation + spriteRegionSize * max(vec2(0), sign(spriteRegionSize)) - vec2(EPSILON)
				);


				vec4 spriteCol = texture2D(u_entityTexture, spriteUV);

				// If pixel is not transparent
				if(spriteCol.a > 0.0)
				{
					// Update depth
					currentPixelDepth = spriteDist;

					// Apply color together with sprite fog
					col = mix(
						u_fogColor, 
						spriteCol.rgb, 
						clamp(16.0 - spriteDist, 0.0, 1.0)
					);
				}
			}
		}
	}

	gl_FragColor = vec4(col, 1.0);
}
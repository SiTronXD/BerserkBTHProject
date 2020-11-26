#version 130

#define MARCH_STEP_SIZE 0.001
#define MARCH_MAX_NUM_STEPS 1000

#define TILE_WALL_COLOR vec3(1.0f)
#define TILE_GOAL_COLOR vec3(0.0f, 1.0f, 0.0f)
#define FOG_COLOR vec3(0.0f, 0.0f, 0.0f)

// Constants
const float PI = atan(-1.0f);
const float FOV = PI * 0.75f;

// Uniforms
uniform sampler2D u_mapTexture;
uniform sampler2D u_wallTexture;
uniform sampler2D u_floorTexture;
uniform sampler2D u_goalTexture;

// vec3(x, y, direction)
uniform vec3 u_camera;

uniform vec2 u_resolution;

uniform float u_timer;

vec2 MAP_SIZE = textureSize(u_mapTexture, 0);
vec2 ONE_OVER_MAP_SIZE = 1.0f / MAP_SIZE;

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
	uv.x *= u_resolution.x / u_resolution.y;
	
	// Keep track of camera in the world
	vec2 camPos = u_camera.xy * ONE_OVER_MAP_SIZE;

	// Create ray
	float dist = 0.001f;
	float screenAngle = uv.x * FOV;
	float angle = -(u_camera.z + screenAngle);	// Flip since y is inverted
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
		vec3 foundCol = texture2D(u_mapTexture, rayPos).rgb;
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
	float fog = clamp(mix(MARCH_MAX_NUM_STEPS * MARCH_STEP_SIZE - dist, 0.0, dist), 0.0, 1.0);
	float halfWallHeight = 0.05 / dist;
	float wall = abs(uv.y) < halfWallHeight ? 1.0f : 0.0f;

	// Find wall uvs
	vec2 hitPos = fract(rayPos * MAP_SIZE);
	vec2 diff = hitPos - 0.5f;
	float isNormalCloseToPositiveDiagonal = dot(diff, vec2(1)) < 0.0f ? 1.0 : 0.0;

	// Choose correct U-value based on if the normal is close to the positive diagonal or not
	float wallU = mix(min(hitPos.x, hitPos.y), max(hitPos.x, hitPos.y), isNormalCloseToPositiveDiagonal);
	float wallV = (uv.y / halfWallHeight) * 0.5f + 0.5f;
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
	float floorFog = clamp(mix(MARCH_MAX_NUM_STEPS * MARCH_STEP_SIZE, 0.0, floorDist * 0.1f), 0.0, 1.0);
	vec3 floorCol = texture2D(
		u_floorTexture,
		fract((camPos * MAP_SIZE + rayDir * floorDist))
	).rgb;


	// Wall/floor is visible
	float showWall = pow(wall * fog, 0.2f);
	float showFloor = pow((1.0 - wall) * floorFog, 0.4f);

	// Wall + floor
	vec3 col = (wallCol * showWall) + (floorCol * showFloor);
	col = mix(
		FOG_COLOR, 
		col, 
		(showWall + showFloor) * (uv.y > halfWallHeight ? 0.0f : 1.0f)
	);

	gl_FragColor = vec4(col, 1.0);
}
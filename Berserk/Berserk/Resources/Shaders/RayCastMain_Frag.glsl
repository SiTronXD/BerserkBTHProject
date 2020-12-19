#version 130

#define MARCH_STEP_SIZE 0.001
#define MARCH_MAX_NUM_STEPS 1000

#define TILE_WALL_COLOR vec3(1.0f)
#define TILE_GOAL_COLOR vec3(0.0f, 1.0f, 0.0f)

// Constants
const float PI = atan(-1.0f);
const float FOV = PI * 0.60f;

// Uniforms
uniform sampler2D u_mapTexture;

uniform vec3 u_cameraPosition;	// vec3(x, y, z)

uniform vec2 u_cameraRotation; // vec2(yaw, roll)
uniform vec2 u_resolution;

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

	float aspectRatio = u_resolution.x / u_resolution.y;
	uv.x *= aspectRatio;
	
	// Rotate uvs
	//uv *= rotate(u_cameraRotation.y);

	// Keep track of camera in the world
	vec2 camPos = u_cameraPosition.xy * ONE_OVER_MAP_SIZE;

	// Create ray
	float dist = 0.0001f;
	float screenAngle = uv.x * FOV;
	float angle = -(u_cameraRotation.x + screenAngle);	// Flip since y is inverted
	vec2 rayDir = vec2(cos(angle), sin(angle));
	vec2 rayPos = camPos + rayDir * dist;

	// March
	bool foundWall = false;
	float foundWallType = 1.0f;
	float stepSize = MARCH_STEP_SIZE * 16.0 * ONE_OVER_MAP_SIZE.x;
	for(int i = 0; i < MARCH_MAX_NUM_STEPS && !foundWall; i++)
	{
		dist += stepSize;
		rayPos += rayDir * stepSize;

		// Sample
		vec3 foundCol = texture2D(u_mapTexture, rayPos).rgb;
		vec3 diff = foundCol - TILE_WALL_COLOR;

		// Hit wall
		if(dot(diff, diff) <= 0.1f)
		{
			foundWall = true;
			foundWallType = 1.0f;
		}

		// Hit goal wall
		diff = foundCol - TILE_GOAL_COLOR;
		if(dot(diff, diff) <= 0.1f)
		{
			foundWall = true;
			foundWallType = 0.0f;
		}
	}

	/*float stepSize = MARCH_STEP_SIZE * 16.0 * ONE_OVER_MAP_SIZE.x;
	for(int i = 0; i < MARCH_MAX_NUM_STEPS && !foundWall; i++)
	{
		dist += stepSize;
		rayPos += rayDir * stepSize;

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
	}*/

	// Find wall uvs
	vec2 hitPos = fract(rayPos * MAP_SIZE);
	vec2 diff = hitPos - 0.5f;
	float isNormalCloseToPositiveDiagonal = dot(diff, vec2(1)) < 0.0f ? 1.0 : 0.0;

	// Choose correct U-value based on if the normal is close to the positive diagonal or not
	float wallU = mix(min(hitPos.x, hitPos.y), max(hitPos.x, hitPos.y), isNormalCloseToPositiveDiagonal);
	
	//vec4(
	//	r = wall texture U-coordinate
	//  g = wallType
	//  b = left 8 bits for depth
	//  a = right 8 bits for depth
	// );

	float stretchedDist = dist * 256.0;

	// Save data in color channels
	gl_FragColor = vec4(
		wallU, 
		foundWallType, 
		floor(stretchedDist) / 256.0, 
		float(int(stretchedDist * 256.0) % 256) / 256.0
	);
}
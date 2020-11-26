#version 130

#define MARCH_STEP_SIZE 0.01

// Constants
const float PI = atan(-1.0f);
const float FOV = PI * 0.5f;

// Uniforms
uniform sampler2D u_mapTexture;

// vec3(x, y, direction)
uniform vec3 u_camera;

uniform vec2 u_resolution;


vec2 ONE_OVER_MAP_SIZE = 1.0f / textureSize(u_mapTexture, 0);


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
	float angle = u_camera.z + uv.x * FOV;
	vec2 rayDir = vec2(cos(angle), sin(angle));
	vec2 rayPos = camPos + rayDir * dist;

	// March
	bool foundWall = false;
	for(int i = 0; i < 100 && !foundWall; i++)
	{
		dist += MARCH_STEP_SIZE;
		rayPos += rayDir * MARCH_STEP_SIZE;

		// Sample
		vec3 foundCol = texture2D(u_mapTexture, rayPos).rgb;
		vec3 diff = foundCol - vec3(1.0f);

		// Hit wall
		if(dot(diff, diff) <= 0.1f)
			foundWall = true;
	}

	float fog = clamp(mix(1.0, 0.0, dist), 0.0, 0.8);
	float wallHeight = 0.07 / dist;
	float wall = abs(uv.y) < wallHeight ? 1.0f : 0.0f;

	vec3 col = vec3(1.0f) * fog * wall;

	gl_FragColor = vec4(col, 1.0);
}
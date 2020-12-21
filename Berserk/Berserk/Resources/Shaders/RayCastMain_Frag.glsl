#version 130

#define MARCH_STEP_SIZE 0.001
#define MARCH_MAX_NUM_STEPS 1000

#define TILE_WALL_COLOR vec3(1.0)
#define TILE_GOAL_COLOR vec3(0.0, 1.0, 0.0)

// Constants
const float PI = atan(-1.0);
const float FOV = PI * 0.6;

// Uniforms
uniform sampler2D u_mapTexture;

uniform vec3 u_cameraPosition;	// vec3(x, y, z)

uniform vec2 u_cameraRotation; // vec2(yaw, roll)
uniform vec2 u_resolution;

vec2 MAP_SIZE = textureSize(u_mapTexture, 0);
vec2 ONE_OVER_MAP_SIZE = 1.0 / MAP_SIZE;

struct HitInfo
{
	float dist;
	float wallType;
};

mat2x2 rotate(float a)
{
	float c = cos(a);
	float s = sin(a);

	return mat2x2(c, s, -s, c);
}

bool isPositionWall(vec2 p, inout float foundWallType)
{
	// Sample
	vec3 foundCol = texture2D(u_mapTexture, p).rgb;
	vec3 diff = foundCol - TILE_WALL_COLOR;

	bool foundWall = false;

	// Hit wall
	if(dot(diff, diff) <= 0.1)
	{
		foundWall = true;
		foundWallType = 1.0;
	}

	// Hit goal wall
	diff = foundCol - TILE_GOAL_COLOR;
	if(dot(diff, diff) <= 0.1)
	{
		foundWall = true;
		foundWallType = 0.0;
	}

	return foundWall;
}

void main()
{
	// Make uvs go from -0.5 to 0.5 (with aspect correction)
	vec2 uv = gl_FragCoord.xy / u_resolution;
	uv -= 0.5;

	float aspectRatio = u_resolution.x / u_resolution.y;
	uv.x *= aspectRatio;
	
	// Create ray
	float screenAngle = uv.x * FOV;
	float angle = -(u_cameraRotation.x + screenAngle);	// Flip since y is inverted
	vec2 rayDir = vec2(cos(angle), sin(angle));

	HitInfo hit;
	hit.dist = 100.0;
	hit.wallType = 1.0;

	// March vertical planes
    if(rayDir.y != 0.0)
    {
		float verticalWallType = 1.0;

        // Step lengths
        float stepX = sign(rayDir.y) / tan(angle);
        float stepY = sign(rayDir.y);
        
        // Initial intersection
        vec2 p = vec2(0.0, round(u_cameraPosition.y + sign(rayDir.y) * 0.5));
        p.x = u_cameraPosition.x + -(u_cameraPosition.y - p.y) / tan(angle);
        
        // March
        for(int i = 0; i < 16; i++)
        {
            int testX = int(p.x);
            int testY1 = int(p.y + 0.5);
            int testY2 = int(p.y - 0.5);
            if(	!isPositionWall(vec2(testX, testY1) * ONE_OVER_MAP_SIZE, verticalWallType) && 
				!isPositionWall(vec2(testX, testY2) * ONE_OVER_MAP_SIZE, verticalWallType))
            {
                p += vec2(stepX, stepY);
            }
            else
                break;
        }
        
		float verticalDist = length(u_cameraPosition.xy - p);
        hit.dist = verticalDist;
		hit.wallType = verticalWallType;
    }
    
    // March horizontal planes
    if(rayDir.x != 0.0)
    {
		float horizontalWallType = 1.0;

        // Step lengths
        float stepX = sign(rayDir.x);
        float stepY = sign(rayDir.x) * tan(angle);
        
        // Initial intersection
        vec2 p = vec2(round(u_cameraPosition.x + sign(rayDir.x) * 0.5), 0.0);
        p.y = u_cameraPosition.y + (p.x - u_cameraPosition.x) * tan(angle);
        
        // March
        for(int i = 0; i < 16; i++)
        {
            int testX1 = int(p.x + 0.5);
            int testX2 = int(p.x - 0.5);
            int testY = int(p.y);
            if(	!isPositionWall(vec2(testX1, testY) * ONE_OVER_MAP_SIZE, horizontalWallType) && 
				!isPositionWall(vec2(testX2, testY) * ONE_OVER_MAP_SIZE, horizontalWallType))
            {
                p += vec2(stepX, stepY);
            }
            else
                break;
        }
        
		// Overwrite distance and wall type, if it should
        float horizontalDist = length(u_cameraPosition.xy - p);
		if(hit.dist > horizontalDist)
		{
			hit.dist = horizontalDist;
			hit.wallType = horizontalWallType;
		}
    }
	hit.dist *= ONE_OVER_MAP_SIZE.x;

	// The fish eye effect is not gonna be fixed here. 
	// The reason being that the entire horizontal fov could be greater than 
	// or equal to 180 degrees depending on the aspect ratio 
	// (-90 in the left window side, to 90 in the right window side)


	// Find wall uvs
	vec2 camPos = u_cameraPosition.xy * ONE_OVER_MAP_SIZE;
	vec2 rayPos = camPos + rayDir * hit.dist;
	vec2 hitPos = fract(rayPos * MAP_SIZE);
	vec2 diff = hitPos - 0.5;
	float isNormalCloseToPositiveDiagonal = dot(diff, vec2(1.0)) < 0.0 ? 1.0 : 0.0;

	// Choose correct U-value based on if the normal is close to the positive diagonal or not
	float wallU = mix(min(hitPos.x, hitPos.y), max(hitPos.x, hitPos.y), isNormalCloseToPositiveDiagonal);
	
	//vec4(
	//	r = wall texture U-coordinate
	//  g = wallType
	//  b = left 8 bits for depth
	//  a = right 8 bits for depth
	// );

	int stretchedDist = int(hit.dist * (65535.0)); // 256.0 * 256.0 - 1.0

	int alphaPart = stretchedDist % 256;
	int bluePart = stretchedDist - alphaPart;

	// Save data in color channels
	gl_FragColor = vec4(
		wallU, 
		hit.wallType, 
		float(bluePart) / (65280.0),	// 256 * 255
		float(alphaPart) / 255.0
	);
}
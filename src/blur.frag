#version 330

// Input attributes from Raylib
in vec2 fragTexCoord;
in vec4 fragColor;

// Output color
out vec4 finalColor;

// Uniforms
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Custom uniform to control dither pixel size
uniform float pixelScale;

// Standard 4x4 Bayer Matrix (values normalized to 0.0 - 1.0 range)
const float bayer4x4[16] = float[16](
    0.0/16.0,  8.0/16.0,  2.0/16.0, 10.0/16.0,
    12.0/16.0,  4.0/16.0, 14.0/16.0,  6.0/16.0,
    3.0/16.0, 11.0/16.0,  1.0/16.0,  9.0/16.0,
    15.0/16.0,  7.0/16.0, 13.0/16.0,  5.0/16.0
    );

// Shader configuration
const float COLOR_LEVELS = 4.0; // Number of distinct shades per channel (e.g. 2.0 = 1-bit / 8 colors, 4.0 = retro depth)
const float SPREAD = 0.75;       // Strength of the dithering pattern blend

void main()
{
  // Sample texture color
  vec4 texColor = texture(texture0, fragTexCoord) * colDiffuse * fragColor;

  // Calculate grid coordinates based on screen position and scale
  vec2 pos = gl_FragCoord.xy / max(pixelScale, 1.0);
  int x = int(mod(pos.x, 4.0));
  int y = int(mod(pos.y, 4.0));
  int index = y * 4 + x;

  // Get threshold value from Bayer matrix (-0.5 to 0.5 range offset)
  float threshold = bayer4x4[index] - 0.5;

  // Add dither threshold offset to RGB channels
  vec3 color = texColor.rgb + threshold * (SPREAD / COLOR_LEVELS);

  // Quantize color into discrete bands
  color = clamp(color, 0.0, 1.0);
  color = floor(color * (COLOR_LEVELS - 1.0) + 0.5) / (COLOR_LEVELS - 1.0);

  finalColor = vec4(color, texColor.a);
}

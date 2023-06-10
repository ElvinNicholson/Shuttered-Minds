// Position of light source
uniform vec2 lightPos;
uniform vec4 color;
uniform float radius;

void main()
{
    //vec2 lightPos = vec2(540, 360);
    vec2 windowSize = vec2(1080, 720);

    // Translate light world position to screen position
    vec2 screenPos = vec2(lightPos.x / windowSize.x, 1 - lightPos.y / windowSize.y);

    // Find the vector representing light source to the current pixel being processed
    vec2 currentPixel = gl_FragCoord.xy / windowSize;
    vec2 lightToFrag = screenPos - currentPixel;
    lightToFrag.y = lightToFrag.y * windowSize.y / windowSize.x;

    // Clamp the length to create the circular effect
    float length = clamp(length(lightToFrag) * radius, 0, 1);
    float darkness = -(length * length) + 1;

    gl_FragColor = vec4(darkness * color.r, darkness * color.g, darkness * color.b, 1);
}

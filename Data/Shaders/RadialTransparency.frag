uniform sampler2D texture;

void main()
{
    vec2 lightPos = vec2(540, 360);
    vec2 windowSize = vec2(1080, 720);
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // Translate light world position to screen position
    vec2 screenPos = vec2(lightPos.x / windowSize.x, 1 - lightPos.y / windowSize.y);

    // Find the vector representing light source to the current pixel being processed
    vec2 lightToFrag = screenPos - (gl_FragCoord.xy / windowSize);
    lightToFrag.y = lightToFrag.y * windowSize.y / windowSize.x;

    // Clamp the length to create the circular effect
    float length = clamp(length(lightToFrag) * 6, 0, 1);
    float darkness = ((1 - length) + 0.1);

    gl_FragColor = vec4(darkness, darkness, darkness, 1 - darkness / 3) * pixel;
}

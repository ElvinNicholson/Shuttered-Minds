uniform sampler2D texture;
uniform sampler2D renderTexture;

void main()
{
    // Get the pixel of current texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // Get the pixel of the baked light texture
    vec4 renderedPixel = texture2D(renderTexture, gl_FragCoord.xy / vec2(1080, 720));

    // Clamp the rpixel of baked light texture
    vec4 clampedPixel = vec4(clamp(renderedPixel.r, 0.3, 1), clamp(renderedPixel.g, 0.3, 1), clamp(renderedPixel.b, 0.3, 1), 1);

    gl_FragColor = clampedPixel * pixel;
}

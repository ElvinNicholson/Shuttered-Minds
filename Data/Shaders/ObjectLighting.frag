uniform sampler2D texture;
uniform sampler2D renderTexture;

void main()
{
    // Get the pixel of current texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // Get the pixel of the baked light texture
    vec4 renderedPixel = texture2D(renderTexture, gl_FragCoord.xy / vec2(1080, 720));

    // Make the alpha of the pixel to be drawn drop off abruptly
    float alpha = clamp(renderedPixel.r * 2, 0, 1);

    // If the alpha is zero, discard this pixel
    if (alpha == 0)
    {
        discard;
    }

    gl_FragColor = pixel * vec4(renderedPixel.r, renderedPixel.g, renderedPixel.b, alpha);
}

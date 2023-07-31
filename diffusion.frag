uniform sampler2D texture;
uniform vec2 resolution;

const int blurRadius=4;

void main()
{
    vec2 texelSize=1./resolution;
    
    vec4 blurColor=vec4(0.);
    for(int i=-blurRadius;i<=blurRadius;i++)
    {
        for(int j=-blurRadius;j<=blurRadius;j++)
        {
            vec2 offset=vec2(i,j)*texelSize;
            blurColor+=texture2D(texture,gl_TexCoord[0].xy+offset);
        }
    }
    
    gl_FragColor=blurColor/float((2*blurRadius+1)*(2*blurRadius+1));
}
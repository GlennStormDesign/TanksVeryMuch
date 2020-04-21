// terrain tile blend fragment shader

uniform sampler2D   tSample;
uniform float       tileAlphaCenter;
uniform float       tileAlphaTopLeft;
uniform float       tileAlphaTop;
uniform float       tileAlphaTopRight;
uniform float       tileAlphaLeft;
uniform float       tileAlphaRight;
uniform float       tileAlphaBottomLeft;
uniform float       tileAlphaBottom;
uniform float       tileAlphaBottomRight;

// begin by averaging surrounding alpha values to center (do in c++)
// find quadrant based on current tex cood
// use quadrant to find two vertical linear interpolations
// use current tex coord y, mix those vertical linear interpolations, to find single horizontal linear interpolation
// use current tex coord x, mix that horizontal linear interpolation to find output alpha for this current tex coord
// apply output alpha to frag color

void main() {

    // coords
    vec2 st = gl_TexCoord[0].xy;
    //int quadrant = (st.x < 0.5) ? (st.y<0.5) ? 0 : 2 : (st.y<0.5) ? 1 : 3; // test that this compound ternary works

    int quadrant = 0;
    if ( st.x < 0.5 )
    {
        if ( st.y < 0.5 )
            quadrant = 0;
        else
            quadrant = 2;
    }
    else
    {
        if ( st.y < 0.5 )
            quadrant = 1;
        else
            quadrant = 3;
    }

    float pctX = st.x * 2.0;
    if ( pctX > 1.0 )
        pctX -= 1.0;
    float pctY = st.y * 2.0;
    if ( pctY > 1.0 )
        pctY -= 1.0;

    // alpha values to linearly interpolate based on quadrant
    float minX = 0.0;
    float maxX = 0.0;

    // use quadrant to find the two vertical linear gradients (A & B) and one horizontal gradient to mix
    if ( quadrant == 0 )
    {
        // YA = topleft to left, YB = top to center
        minX = mix( tileAlphaTopLeft, tileAlphaLeft, pctY );
        maxX = mix( tileAlphaTop, tileAlphaCenter, pctY );
    }
    else if ( quadrant == 1 )
    {
        // YA = top to center, YB = topright to right
        minX = mix( tileAlphaTop, tileAlphaCenter, pctY );
        maxX = mix( tileAlphaTopRight, tileAlphaRight, pctY );
    }
    else if ( quadrant == 2 )
    {
        // YA = left to bottomleft, YB = center to bottom
        minX = mix( tileAlphaLeft, tileAlphaBottomLeft, pctY );
        maxX = mix( tileAlphaCenter, tileAlphaBottom, pctY );
    }
    else
    {
        // YA = center to bottom, YB = right to bottomright
        minX = mix( tileAlphaCenter, tileAlphaBottom, pctY );
        maxX = mix( tileAlphaRight, tileAlphaBottomRight, pctY );
    }

    // color lookup
    vec4 px = texture2D( tSample, gl_TexCoord[0].xy );

    // output alpha value
    px.a *= mix( minX, maxX, pctX );

    // color manipulation
    gl_FragColor = gl_Color * px;

}

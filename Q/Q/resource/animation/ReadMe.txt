File Format of an Animation

<playrate> <is looping> <use texel>

use texel:
	[<texture name> <texelX> <texelY> <texelWidth> <texelHeight> <time amount to be played in seconds>]...

!use texel:
	[<texture name> <time amount to be played in seconds>]...


`is looping` and `use texel` should be a boolean represented either `true` or `false`

`playrate`, `time amount to be played in seconds` are of float.
`texelX`, `texelY`, `texelWidth`, `texelHeight` are of int.
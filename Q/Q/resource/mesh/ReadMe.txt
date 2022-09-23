File Format of a Mesh:

<vertex count> <index count> <primitive type>
[<v | u | i> <arguments>]...


primitive type
	points | lines | lines_strip | line_loop | triangles | triangle_strip | triangle_fan


v: a vertex position in NDC
arguments
	x coord, y coord

u: uv coordinates
arguments
	x coord, y coord
	
i: a vertex index
arguments
	index
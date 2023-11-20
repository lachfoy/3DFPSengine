import bpy
import bmesh

# Ensure we are in object mode
bpy.ops.object.mode_set(mode='OBJECT')

# Get the active mesh object
obj = bpy.context.active_object

# Check if the object is a mesh
if obj.type == 'MESH':
    mesh = obj.data
else:
    raise ValueError("Active object is not a mesh")


# Create a BMesh from the mesh
bm = bmesh.new()
bm.from_mesh(mesh)

# Extract vertices
vertices = [vert.co for vert in bm.verts]

# Extract edges
edges = [(edge.verts[0].index, edge.verts[1].index) for edge in bm.edges]

# Free the BMesh
bm.free()


# Define the export file path
export_path = bpy.path.abspath("//nav_graph.txt")

# Export vertices and edges to a file
with open(export_path, 'w') as f:
    # Write vertices
    f.write("# Vertices\n")
    for idx, vertex in enumerate(vertices):
        # Swap from blender to OpenGL coordinates here as well
        f.write(f"{vertex.x} {vertex.z} {-vertex.y}\n")

    # Write edges
    f.write("\n# Edges\n")
    for edge in edges:
        f.write(f"{edge[0]} {edge[1]}\n")

print(f"Graph exported to {export_path}")

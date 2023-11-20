import bpy
import bmesh

# Ensure we are in object mode
bpy.ops.object.mode_set(mode='OBJECT')

# Get the active mesh object
obj = bpy.context.active_object

dimensions = obj.dimensions

# Get the dimensions of the plane
width = dimensions.x
height = dimensions.y

# Get location
location = obj.location

nodeSize = 0.5

def makeNode(x, y):
    bpy.ops.mesh.primitive_cube_add(size=nodeSize, enter_editmode=False, align='WORLD', location=(x, y, 0))

space = 2

startX = 0
startY = 0

gridWidth = 10
gridHeight = 10

for i in range(0, gridWidth):
    for j in range(0, gridHeight):
        makeNode(startX + i * space, startY + j * space)
        

# Select all objects whose names start with 'Plane_'
#planes = [obj for obj in bpy.data.objects if obj.name.startswith('Plane_')]




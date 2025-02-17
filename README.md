# Shadow-Generation-based-on-Shadow-Volume
This project implements real-time shadow generation using the Shadow Volume technique in OpenGL. The method generates shadows by creating geometric volumes that represent shadowed spaces in a 3D environment, providing pixel-perfect shadow boundaries without common resolution-dependent artifacts.
Core Implementation:
The technique employs a multi-pass rendering approach utilizing the stencil buffer to determine shadowed regions. The process involves:

# 1.Scene and Volume Generation:
•	Create a 3D environment with dynamic lighting and camera controls
•	Generate shadow volumes by extruding object geometry from the light source
•	Update volumes in real-time as the light source moves
# 2.Shadow Determination:
•	Utilize stencil buffer operations for shadow calculations
•	First pass: Render the scene from the light's perspective, writing to the stencil buffer
•	Second pass: Render from the camera's viewpoint, testing each pixel against the stencil buffer
•	Pixels inside shadow volumes (indicated by stencil buffer values) are rendered as shadowed
# 3.Final Rendering:
•	Apply appropriate shading based on stencil test results
•	Darken areas determined to be in shadow
•	Maintain proper lighting for non-shadowed regions
•	Ensure smooth transitions during light movement
The implementation focuses on achieving both visual accuracy and performance efficiency. Key objectives include maintaining consistent frame rates while providing accurate shadow boundaries and proper self-shadowing capabilities. This approach demonstrates practical applications of advanced shadow rendering techniques in modern real-time graphics applications


# 2、Theory and Algorithm
## 2.1 The main algorithms are listed as follows：
（1）Shadow Volume Theory
Shadow volumes operate on a fundamental principle: constructing a volume that encompasses the space occluded from a light source. This volume is created by extruding object silhouettes away from the light source, effectively creating a geometric representation of shadowed space. The algorithm identifies the boundaries between lit and shadowed regions by determining where view rays intersect these volumes, providing a precise method for real-time shadow generation.
#（2）Core Components
## 2.2.1 Stencil Buffer Algorithm
The implementation uses a stencil buffer-based approach that operates in two main phases. First, the algorithm clears the stencil buffer and initializes it for shadow determination. During rendering, it tracks ray intersections with shadow volumes using stencil operations. When a ray enters a shadow volume, the stencil value increases; when it exits, the value decreases. 
## 2.2.2 Volume Generation Algorithm
The shadow volume generation process follows a systematic approach:
•	Face Normal Calculation: For each triangle in the object, compute its normal vector
•	Silhouette Edge Detection: Determine which edges form the silhouette from the light's perspective
•	Volume Extrusion: Create the shadow volume by extending these silhouette edges away from the light
•	Face Generation: Construct the volume faces by connecting original and extruded vertices
The algorithm dynamically updates these volumes as the light source moves, ensuring accurate real-time shadow rendering. Provides pixel-perfect shadows without the resolution-dependent artifacts common in other shadow techniques, making it particularly suitable for applications requiring high-quality shadow rendering.


# 3、Implementation 
## 3.1 Algorithm Design and Implementation
### Design Overview
The implementation focuses on creating realistic real-time shadows using the shadow volume technique in OpenGL. The algorithm employs a three-pass rendering system to achieve accurate shadow rendering. The first pass renders the scene normally, establishing the base geometry and lighting. The second pass generates and renders shadow volumes using stencil buffer operations, where front-facing polygons increment the stencil buffer and back-facing polygons decrement it.

The core algorithm includes efficient shadow volume generation by identifying silhouette edges and extruding them away from the light source. Camera movement is handled through a robust control system supporting six degrees of freedom (forward, backward, left, right, rotation, and zoom), implemented using GLFW keyboard and mouse input handling. Core Algorithms and Implementation

![29f71d01f887f3770adba6c42b40abe](https://github.com/user-attachments/assets/47f21c4b-fb47-4c11-95cb-8889f48ad8bc)
![c80b03623337f26dddb36877fd45451](https://github.com/user-attachments/assets/dc636142-be80-44b1-9cb8-3635416f4447)
![fdb6ee80ab6463eafdc4e3669007e42](https://github.com/user-attachments/assets/fe98233a-b0f3-435d-b386-2f0f0eb67aba)
![e5eab47a73dfe76755524be4203ce89](https://github.com/user-attachments/assets/80d302fa-f4d1-4dc2-86fc-3a03d11afce4)
![86a2a17914953da74d1268f61f96dc2](https://github.com/user-attachments/assets/632526c9-166f-4777-8b85-3294d8a8b61e)







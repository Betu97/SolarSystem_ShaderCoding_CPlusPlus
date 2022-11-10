#Explanation:

This project shows a Solar System with the sun in the center. The specular color and diffuse color algorithm has been implemented in the fragment shader, achieving lighting with shininess.
In addition the lighting is dynamic from the center of the universe (the sun) making it move in circles defeating the speed of rotation and translation.
In each "DrawPlanet" there is a u_light_dir that is moved.

A slight movement of the camera has been made with the mouse when moving within the program. It has been done as if the user was in a ship looking at the universe, so it is a little movement simulating the head.

The planets have implemented a different rotation and translation and with different speeds.

Each planet has its textures uploaded from a planet textures page.

There is a sphere skybox created with the textures of the universe. Both the sun and the skybox have their own fragment and vertex shader so they don't have shadows.

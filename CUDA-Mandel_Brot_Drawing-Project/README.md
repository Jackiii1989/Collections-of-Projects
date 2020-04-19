# Abbreviations

GPU - GRAPHICAL PROCESS UNIT

# Brief Description <h1> 
 
This is was a subject to use graphical cards to speed-up the solve certain problems. The challange was to draw madendel Brot pictures. Different ideas and approaches has been tried to adapt the software to the hardware for better performance. Sadly the pictures were to big to upload them. The application uses the GPU from NVIDIA to speedup the process. Each thread is handling one pixel ("a lot of work in the flight").
 
 The best results to speed-up the application are: 
 - using pinned-memory to not let the allocate memory get swapped
 - algorithmic chang: adapt the terminal condition for the loop
 - making use of good memory access pattern (to uncessary fetch to many times)


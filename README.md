### COMP 4411 Project3 Tracer
[Project homepage](https://course.cse.ust.hk/comp4411/Password_Only/projects/trace02/index.html)

***
### Required
 - [ ] Box Intersection
 - [ ] Phong specular-reflection model
 - [ ] Multiple light sources, 
 - [x]  (Fengyee) directional lights, point lights
 - [x]  (Fengyee) Shadow Attenuation
 - [x]  (Fengyee) Reflection
 - [x]  (Fengyee) Refraction

*** 
#### Bells and Whistles

In many cases, the material in the Foley, et al. book goes into more depth on a particular subject than the Alan Watt text. In addition, Glassner's book on ray tracing is a very comprehensive exposition of a whole bunch of ways ray tracing can be expanded or optimized (and it's really well written). If you're planning on implementing any of these bells and whistles, you are encouraged to read the relevant sections in these books as well.

Remember that you'll need to establish to our satisfaction that you've implemented the extension! You should have test cases that clearly demonstrate the effect of the code you've added to the ray tracer. Sometimes different extensions can interact, making it hard to tell how each contributed to the final image, so it's also helpful (that is, necessary!) to add controls to selectively enable and disable your extensions.

* [x]  (Fengyee) Implement an adaptive termination criterion for tracing rays, based on ray contribution. Control the adaptation threshold with a slider.

    - [x]  (Fengyee) Implement antialiasing by supersampling and averaging down.  You must provide a slider that indicates the number of sub-pixel samples being used (e.g., something in the range 1-5 indicating 1x1, 2x2, ... 5x5 supersampling). This whistle upgrades to a bell if you implement the first stage of distribution ray tracing by jittering the sub-pixel samples.  The noise introduced by jittering should be evident when casting 1 ray per pixel.

* [x]  (Fengyee) Implement spot lights.

    - [ ]  (Zhaolucen) Add a menu option that lets you specify a background image to replace the environment's ambient color during the rendering.  That is, any ray that goes off into infinity behind the scene should return a color from the loaded image, instead of just black.  The background should appear as the backplane of the rendered image with suitable reflections and refractions to it.

* [ ]  (Fengyee) Find a good way to accelerate shadow attenuation.  Do you need to check against every object when casting the shadow ray?  This one is hard to demonstrate directly, so be prepared to explain in detail how you pulled it off.  

* [ ] Deal with overlapping objects intelligently.  While the skeleton code handles materials with arbitrary indices of refraction, it assumes that objects don't intersect one another. It breaks down when objects intersect or are wholly contained inside other objects. Add support to the refraction code for detecting this and handling it in a more realistic fashion.  Note, however, that in the real world, objects can't coexist in the same place at the same time. You will have to make assumptions as to how to choose the index of refraction in the overlapping space.  Make those assumptions clear when demonstrating the results.

* [ ]  (Zhaolucen) Add texture mapping support to the program. The most basic kind of texture mapping is to apply the map to the diffuse color of a surface. But many other parameters can be mapped. Reflected color can be mapped to create the sense of a surrounding environment. Transparency can be mapped to create holes in objects. Additional (variable) extra credit will be given for such additional mappings.  You will need to modify the parser for handling texture mapping inputs the types of mapping mentioned above.  Additional credit will be awarded for quality implementation of texture mapping on general trimeshes.

* [x]  (Fengyee) Implement antialiasing by adaptive supersampling, as described in Foley, et al., 15.10.4. For full credit, you must show some sort of visualization of the sampling pattern that results.  
> Not used: For example, you could create another image where each pixel is given an intensity proportional to the number of rays used to calculate the color of the corresponding pixel in the ray traced image.  Implementing this bell/whistle is a big win -- nice antialiasing at low cost.

* [x]  (Fengyee) Implement more versatile lighting controls, such as the Warn model described in Foley 16.1.5. This allows you to do things like control the shape of the projected light.

    - [ ] Implement bump mapping (Watt 8.4; Foley, et al. 16.3.3).

* [ ] Implement solid textures or some other form of procedural texture mapping, as described in Foley, et al., 20.1.2 and 20.8.3. Solid textures are a way to easily generate a semi-random texture like wood grain or marble.

* [ ] Add some new types of geometry to the ray tracer. Consider implementing torii or general quadrics. Many other objects are possible here.

    - [ ] Add support for height-fields. Click here for a discussion on what they are and how they can be generated.  

        + [ ] for first, for each additional
            Implement distribution ray tracing to produce one or more or the following effects: depth of field, soft shadows, motion blur, or glossy reflection (See Watt 10.6, Glassner, chapter 5, or Foley, et al., 16.12.4).

- [ ] Add some higher-level geometry to the ray tracer, such as surfaces of revolution, extrusions, metaballs or blend surfaces.  You may have implemented one or more of these as a polygonal object in the modeler project.  For the Raytracer, be sure you are actually raytracing the surface as a mathematical construct, not just creating a polygonal representation of the object and tracing that.  Yes, this requires lots of complicated math, but the final results are definitely worth it (see Transparent Metaballs).  A really good tutorial on raytracing metaballs.

- [ ] Implement ray-intersection optimization by either implement the BSP Tree implemented or by implementing a different optimization method, such as hierarchical bounding volumes (See Glassner 6.4 and 6.5, Foley, et al., 15.10.2).

- [ ] Implement a more realistic shading model. Credit will vary depending on the sophistication of the model. A simple model factors in the Fresnel term to compute the amount of light reflected and transmitted at a perfect dielectric (e.g., glass). A more complex model incorporates the notion of a microfacet distribution to broaden the specular highlight. Accounting for the color dependence in the Fresnel term permits a more metallic appearance. Even better, include anisotropic reflections for a plane with parallel grains or a sphere with grains that follow the lines of latitude or longitude. Sources: Watt, Chapter 7, Foley et al, Section 16.7; Glassner, Chapter 4, Section 4; Ward's SIGGRAPH '92 paper; Schlick's Eurographics Rendering Workshop '93 paper.

    This all sounds kind of complex, and the physics behind it is. But the coding doesn't have to be. It can be worthwhile to look up one of these alternate models, since they do a much better job at surface shading.  Be sure to demo the results in a way that makes the value added clear.

    Theoretically, you could also invent new shading models. For instance, you could implement a less realistic model! Could you implement a shading model that produces something that looks like cel animation? Variable extra credit will be given for these "alternate" shading models. 

* [ ] Implement CSG, constructive solid geometry. This extension allows you to create very interesting models. See page 108 of Glassner for some implementation suggestions.

    - [ ] Add a particle systems simulation and renderer (Foley 20.5, Watt 17.7).

        + [ ] Implement caustics.  Caustics are variations in light intensity caused by refractive focusing--everything from simple magnifying-glass points to the shifting patterns on the bottom of a swimming pool.  An introduction, and a paper discussing a ray-trace project that included caustics.
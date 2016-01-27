
Sprite Compiler


"Compiled Sprites" is an advanced programming technique used to maximize
the blit performance of a two dimensional sprite. The concept is similar
to the one employed by DIVE in that known parameters of the blit operation
are used to generate a custom blitter that is optimized for the given
parameters. In the case of DIVE, the blitter is optimized for a given
source and destination image type and size. A sprite compiler would
optimize the blitter further by using the actual image data to generate a
blitter that is specific to that image. In the context of a game, the
sprite compiler would be used to generate a small, fully optimized blitter
for every sprite in the game. Each sprite could then be considered a self
contained object ( a Compiled Sprite ) that would blit itself onto the
destination buffer.

Where this technique becomes most valuable is when blitting irregularly
shaped or partially transparent sprites. With this kind of sprite, the
blitter would normally have to test each pixel of the sprite and copy it,
only if it is not transparent. Processing a conditional branch instruction
for every pixel in the sprite would dramatically increase the amount of
time required to blit. When using a compiled sprite, the blitter already
knows which pixels are transparent and adjusts the source and destination
pointers accordingly.

In a PCDOS world, where the properties of the display mode are known (
probably Mode 13 or Mode X ), the sprite compiler would generate a blitter
that could blit the sprite directly to video memory. Under a GUI like
Warp, the compiled sprite could be designed to blit to an off screen
buffer, where the entire scene is rendered before being copied to the
screen by DIVE ( double buffering ).



Beehive

The purpose of this application is to demonstrate the power of compiled
sprites by comparing the performance of a typical sprite algorithm against
a compiled sprite.

When the application first starts, it will display a single sprite moving
in a random path around the application window. The user can add sprites
by holding down the 'H' key. As the sprite count increases, the effects on
the frame rate will become evident. The actual frame rate will be
displayed on the title bar.

The user will be able to switch the application from compiled sprites to
normal sprites from a pull down menu. As the number of sprites increases,
the difference in performance from compiled to normal sprites will become
more dramatic.


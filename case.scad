jack_count = 2;
usb_h = 16.5;
usb_v = 14;

jack_width = 25;
jack_depth = 25;
interior_width = 35 + jack_depth + 3;
interior_length = jack_width * jack_count;
interior_height = jack_width;
wall_thickness = 3;

$fn=30;

module rcube(x, y, z, r)
{
    translate([r, r, 0])
    linear_extrude(height = abs(z))
    offset(r)
    square([x - (r * 2), y - (r * 2)]);
}

module jack()
{
    translate([wall_thickness + 25/2, wall_thickness, wall_thickness + (25/2)])
    rotate([-90, 0, 0])
    {
        translate([0, 0, -11])
        cylinder(d=10, h=12);
        cylinder(d=25, h=25);
    }
}

module usb()
{
    usbsize = 12.5;
    translate([
        -2,
        -usb_h + (usbsize / 2) + interior_width + wall_thickness - usbsize ,
        usb_v -  (usbsize / 2) + wall_thickness
    ])
    cube([usbsize, usbsize, usbsize]);
}

// case bottom
difference() 
{
    // outer body
    rcube(
        interior_length + (2 * wall_thickness), 
        interior_width + (2 * wall_thickness),
        interior_height + (1.5 * wall_thickness),
        wall_thickness);

//    // top cut for lid
    translate([wall_thickness/2, wall_thickness/2, interior_height + wall_thickness])
    rcube(
        interior_length + (wall_thickness), 
        interior_width + (wall_thickness),
        wall_thickness * 2,
        wall_thickness);

    // interior cut
    translate([wall_thickness, wall_thickness, wall_thickness])
    cube(size=[
        interior_length, 
        interior_width,
        interior_height + 2 * wall_thickness]);

    // jack cutouts
    for (i = [0:1:jack_count-1])
    {
        translate([i * jack_width, 0, 0])
        jack();
    }
    
    // USB cutout
    usb();

}

// Lid
translate([0, interior_width + wall_thickness * 4, 0])
union()
{
    rcube(
        interior_length + (wall_thickness * 2), 
        interior_width + (wall_thickness * 2),
        wall_thickness/2,
        wall_thickness);

    translate([wall_thickness / 2, wall_thickness / 2, 0])
    rcube(
        interior_length + wall_thickness, 
        interior_width + wall_thickness,
        wall_thickness,
        wall_thickness);
}
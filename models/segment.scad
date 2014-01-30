radius = 18;
height = 22;
length = 70;
wall   =  2;

$fn=100;

module post() {
    difference() {
        cylinder(r=4, h=height-wall);
        cylinder(r=1.5, h=4);
    }
}


difference() {
    hull() {
        cylinder(r=radius, h=height);

        translate([0, length, 0])
        cylinder(r=radius, h=height);
    }

    hull() {
        translate(0, wall, 0)
        cylinder(r=radius-wall, h=height-wall);

        translate([0, length, 0])
        cylinder(r=radius-wall, h=height-wall);
    }
}

translate([0, -(radius+wall)/2-2*wall, 0])
post();

translate([0, length + ((radius+wall)/2+2*wall), 0])
post();
$fn=100;

button_radius = 49;
hole_radius = 14.5;
table_padding = 40;
extra_padding = 2;
bottom_padding = 30;

wall_thickness = 4;

mount_height = table_padding + extra_padding + button_radius + hole_radius + bottom_padding;
mount_depth = 50;
mount_width = (hole_radius * 2) + (bottom_padding * 2);


// difference() {
//     cube(size=[mount_width, mount_height, mount_depth]);

//     translate([wall_thickness, wall_thickness, wall_thickness])
//     cube(size=[mount_width - (wall_thickness*2), mount_height, mount_depth]);
// }

module screw_mount() {
    difference() {
        cube([10, wall_thickness, 10]);

        rotate([90, 0, 0])
        translate([5, 5, -wall_thickness-1])
        cylinder(r=2, h=wall_thickness+2);
    }
}


module bottom() {
    difference() {
    hull() {
        // lower left
        translate([wall_thickness*2, wall_thickness*2, 0])
        cylinder(r=wall_thickness*2, h=wall_thickness);
        
        // lower right
        translate([mount_width-(wall_thickness*2), wall_thickness*2, 0])
        cylinder(r=wall_thickness*2, h=wall_thickness);

        // upper right
        translate([mount_width-(wall_thickness*2), mount_height-(wall_thickness*2), 0])
        cube(size=[wall_thickness*2, wall_thickness*2, wall_thickness]);
        
        // upper left
        translate([0, mount_height-(wall_thickness*2), 0])
        cube(size=[wall_thickness*2, wall_thickness*2, wall_thickness]);
    }

    union() {
        translate([mount_width/2, wall_thickness+7.5, -1])
        cylinder(r=3, h=wall_thickness+2);

        translate([mount_width/2-4.5, wall_thickness+7.5-2.5, wall_thickness-.8])
        cube(size=[9, 5, 1]);
    }
    }
}

module sides() {
    difference() {
        hull() {
            // lower left
            translate([wall_thickness*2, wall_thickness*2, 0])
            cylinder(r=wall_thickness*2, h=mount_depth);
            
            // lower right
            translate([mount_width-(wall_thickness*2), wall_thickness*2, 0])
            cylinder(r=wall_thickness*2, h=mount_depth);

            // upper right
            translate([mount_width-(wall_thickness*2), mount_height-(wall_thickness*2), 0])
            cube(size=[wall_thickness*2, wall_thickness*2, mount_depth]);
            
            // upper left
            translate([0, mount_height-(wall_thickness*2), 0])
            cube(size=[wall_thickness*2, wall_thickness*2, mount_depth]);
        }
        
        hull() {
            // lower left
            translate([wall_thickness*2, wall_thickness*2, 0])
            cylinder(r=wall_thickness, h=mount_depth+1);
            
            // lower right
            translate([mount_width-(wall_thickness*2), wall_thickness*2, 0])
            cylinder(r=wall_thickness, h=mount_depth+1);

            // upper right
            translate([mount_width-(wall_thickness*2), mount_height-(wall_thickness*2), 0])
            cube(size=[wall_thickness, wall_thickness, mount_depth+1]);
            
            // upper left
            translate([wall_thickness, mount_height-(wall_thickness*2), 0])
            cube(size=[wall_thickness, wall_thickness, mount_depth+1]);
        }
    }

    intersection() {
        translate([wall_thickness, wall_thickness, 0])
        cube([mount_width-(wall_thickness*2), mount_height-(wall_thickness*2), mount_depth]);

        union() {
            translate([wall_thickness, wall_thickness, mount_depth-6-wall_thickness])
            cylinder(r=9, h=6);
            translate([wall_thickness, wall_thickness, mount_depth-10-wall_thickness])
            cylinder(r1=3, r2=9, h=4);

            translate([mount_width-wall_thickness, wall_thickness, mount_depth-6-wall_thickness])
            cylinder(r=9, h=6);
            translate([mount_width-wall_thickness, wall_thickness, mount_depth-10-wall_thickness])
            cylinder(r1=3, r2=9, h=4);
            
            translate([mount_width-wall_thickness, mount_height-wall_thickness, mount_depth-6-wall_thickness])
            cylinder(r=9, h=6);
            translate([mount_width-wall_thickness, mount_height-wall_thickness, mount_depth-10-wall_thickness])
            cylinder(r1=3, r2=9, h=4);

            translate([wall_thickness, mount_height-wall_thickness, mount_depth-6-wall_thickness])
            cylinder(r=9, h=6);
            translate([wall_thickness, mount_height-wall_thickness, mount_depth-10-wall_thickness])
            cylinder(r1=3, r2=9, h=4);
        }
    }
}

module cover() {
    hole_center = mount_height - table_padding - button_radius;

    difference() {
        hull() {
            // lower left
            translate([wall_thickness*2, wall_thickness*2, 0])
            cylinder(r=wall_thickness, h=wall_thickness);
            
            // lower right
            translate([mount_width-(wall_thickness*2), wall_thickness*2, 0])
            cylinder(r=wall_thickness, h=wall_thickness);
            
            // upper right
            translate([mount_width-(wall_thickness*2), mount_height-(wall_thickness*2), 0])
            cube(size=[wall_thickness, wall_thickness, wall_thickness]);
            
            // upper left
            translate([wall_thickness, mount_height-(wall_thickness*2), 0])
            cube(size=[wall_thickness, wall_thickness, wall_thickness]);
        }
        
        translate([mount_width/2, hole_center, -1])
        cylinder(r=hole_radius, h=wall_thickness*2);
    }

    // Fake button
    // translate([mount_width/2, hole_center, 5])
    // cylinder(r=button_radius, h=20);
}

module cover_screws() {
    translate([wall_thickness+3.5, wall_thickness+3.5, mount_depth/2])
    cylinder(r=1.5, h=30);

    translate([mount_width-wall_thickness-3.5, wall_thickness+3.5, mount_depth/2])
    cylinder(r=1.5, h=30);

    translate([mount_width-wall_thickness-3.5, mount_height-wall_thickness-3.5, mount_depth/2])
    cylinder(r=1.5, h=30);

    translate([wall_thickness+3.5, mount_height-wall_thickness-3.5, mount_depth/2])
    cylinder(r=1.5, h=30);
}

module mount_screws() {
    spacing = 15;
    
    translate([spacing, mount_height, spacing])
    rotate([90, 0, 0])
    cylinder(r=2, h=wall_thickness+2);

    translate([spacing, mount_height, mount_depth-spacing])
    rotate([90, 0, 0])
    cylinder(r=2, h=wall_thickness+2);

    translate([mount_width-spacing, mount_height, mount_depth-spacing])
    rotate([90, 0, 0])
    cylinder(r=2, h=wall_thickness+2);

    translate([mount_width-spacing, mount_height, spacing])
    rotate([90, 0, 0])
    cylinder(r=2, h=wall_thickness+2);
}

bottom();
difference() {
    sides();
    cover_screws();
    mount_screws();
}

// difference() {
//     translate([0, 0, mount_depth-wall_thickness])
//     cover();
//     screws();
// }

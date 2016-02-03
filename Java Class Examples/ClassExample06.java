/*
 * ClassExample06: Simplified - no need to store intermediate calculations
 *
 * This example builds on ClassExample05 and illustrates how constructors can be overloaded.
 *
 * Each example in this series, ClassExample01 thorugh ClassExample12, is meant to show one or two additional
 * concepts that move from the most basic example of a class (ClassExample01) to a more typical class that
 * one would normally expect.
 *
 * A small improvement is made next--we do not need to create a special variable that we are only
 * going to use once to store the volume.  Instead, we can simply return it directly.
 */

// Define 'Box' class of objects

class Box {

    // Declare class variables

    double width;
    double height;
    double depth;

    // Declare class methods

    double volume() {
        return (width * height * depth);  // Directly calculate and return the box's volume
    }

}

// Main class

class ClassExample06 {

    public static void main(String args[]) {

        // Initialize Variables

        Box myBox1 = new Box();
        Box myBox2 = new Box();

        // Set values

        myBox1.width  = 10;
        myBox1.height = 20;
        myBox1.depth  = 15;

        myBox2.width  =  3;
        myBox2.height =  6;
        myBox2.depth  =  9;

        // Calculate and print volume of the boxes

        System.out.println("Volume of box 1 is " + myBox1.volume());
        System.out.println("Volume of box 2 is " + myBox2.volume());

    }
}

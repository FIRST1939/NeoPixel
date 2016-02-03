/*
 * ClassExample04: Objects can have methods
 *
 * This example builds on ClassExample03 and illustrates how constructors can be overloaded.
 *
 * Each example in this series, ClassExample01 thorugh ClassExample12, is meant to show one or two additional
 * concepts that move from the most basic example of a class (ClassExample01) to a more typical class that
 * one would normally expect.
 *
 * An important concept in software development is not to repeat ourselves.  So, now that we have a box class
 * we can 'teach the box' (so to speak) how to calculate it's own volume.  This way we do not have to repeat
 * the formula for calculating the volume of the box.  Now, we can simple "ask" the box what its volume is
 * by using a new method called 'volume'.
 */

// Define 'Box' class of objects

class Box {

    // Declare class variables

    double width;
    double height;
    double depth;

    // Declare class methods

    void volume() {
        System.out.print("Volume is ");
        System.out.println(width * height * depth);
    }

}

// Main class

class ClassExample04 {

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

        myBox1.volume();  // "Ask" box one what its volume is
        myBox2.volume();  // And, box two

    }
}

/*
 * ClassExample02: Instantiating two instances of an object
 *
 * This example builds on ClassExample01 and illustrates how constructors can be overloaded.
 *
 * Each example in this series, ClassExample01 thorugh ClassExample12, is meant to show one or two additional
 * concepts that move from the most basic example of a class (ClassExample01) to a more typical class that
 * one would normally expect.
 *
 * Now that we have a class, we can use that class multiple times without having to define over and over
 * unique variables for each time we use the class.  When you 'use' a class, this is called creating an
 * instance of the class.
 */

// Define 'Box' class of objects

class Box {

    // Declare class variables

    double width;
    double height;
    double depth;
}

// Main class

class ClassExample02 {

    public static void main(String args[]) {

        // Initialize Variables

        Box myBox1 = new Box();  // Create one box
        Box myBox2 = new Box();  // Create another different box
        double vol;

        // Set values

        myBox1.width  = 10;
        myBox1.height = 20;
        myBox1.depth  = 15;

        myBox2.width  =  3;
        myBox2.height =  6;
        myBox2.depth  =  9;

        // Calculate and print volume of the boxes

        vol = myBox1.width * myBox1.height * myBox1.depth;
        System.out.println("Volume is of box 1 is " + vol);


        vol = myBox2.width * myBox2.height * myBox2.depth;
        System.out.println("Volume is of box 2 is " + vol);

    }
}

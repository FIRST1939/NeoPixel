/*
 * ClassExample01: A bare-bones object
 *
 * Each example in this series, ClassExample01 thorugh ClassExample11, is meant to show one or two additional
 * concepts that move from the most basic example of a class (ClassExample01) to a more typical class that
 * one would normally expect.
 *
 * In this example, we will create about as basic of a class as possible--a class that only contains
 * attributes.
 */

// Define a new class of objects called 'Box' that defines a three-dimensional box.

class Box {

    // Define the attributes of our class (our class variables)

    double width;   // The width of our box ...
    double height;  //    ... along with it's height,
    double depth;   //    ... and it's depth.

}

// And, this class is our main program

class ClassExample01 {

    public static void main(String args[]) {

        // Initialize Variables

        Box myBox = new Box();   // The box (NOTE: our class 'box' is really a new type that we created)
        double vol;              // A place to store the volume we calculated

        // Set values

        myBox.width  = 10;       // We are just making these values up and they are just
        myBox.height = 20;       // for illustration purposes
        myBox.depth  = 15;

        // Calculate volume (by using each of the box's attributes)

        vol = myBox.width * myBox.height * myBox.depth;

        // Print results

        System.out.println("Volume is " + vol);
    }
}

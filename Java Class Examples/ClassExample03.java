/*
 * ClassExample03: Assigning an object to two variables -- both point to the same object
 *
 * This example builds on ClassExample02 and illustrates how constructors can be overloaded.
 *
 * Each example in this series, ClassExample01 thorugh ClassExample12, is meant to show one or two additional
 * concepts that move from the most basic example of a class (ClassExample01) to a more typical class that
 * one would normally expect.
 *
 * 'new' creates a new box.  If we assign the box we created to another variable of type 'Box' it does not
 * create a new box.  Instead, both variables refer to the same box.  Both "point to" the one instance you
 * have created.  This example illustrates that even though we have two variables, 'myBox1' and 'myBox2',
 * both are actually just two different names for the same box.  So, if we set the dimensions of one we are
 * also setting the dimensions of the other because the are really just the same.
 */

// Define 'Box' class of objects

class Box {

    // Declare class variables

    double width;
    double height;
    double depth;
}

// Main class

class ClassExample03 {

    public static void main(String args[]) {

        // Initialize Variables

        Box myBox1 = new Box();   // Create our first new box
        Box myBox2 = myBox1;      // Instead of creating a 'new' one, assign the first to myBox2
        double vol;

        // Set values

        myBox1.width  = 10;   // Here we think we are setting the
        myBox1.height = 20;   // dimensions to the first box
        myBox1.depth  = 15;

        myBox2.width  =  3;   // And, here we think we are setting them for
        myBox2.height =  6;   // the second box.  But, instead we are really
        myBox2.depth  =  9;   // just changing our single box's dimensions.

        // Calculate and print volume of the boxes

        vol = myBox1.width * myBox1.height * myBox1.depth;
        System.out.println("Volume is of box 1 is " + vol);


        vol = myBox2.width * myBox2.height * myBox2.depth;
        System.out.println("Volume is of box 2 is " + vol);

    }
}

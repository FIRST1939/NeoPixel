/*
 * ClassExample07: Using setters to hide (encapsulate) local variables
 *
 * This example builds on ClassExample06 and illustrates how constructors can be overloaded.
 *
 * Each example in this series, ClassExample01 thorugh ClassExample12, is meant to show one or two additional
 * concepts that move from the most basic example of a class (ClassExample01) to a more typical class that
 * one would normally expect.
 *
 */

// Define 'Box' class of objects

class Box {

    // Declare class variables
    //
    // NOTE: These are now 'private' which means only our 'Box' class knows about them.  Their
    //       values can only be set, retrieved, used, or manipulated by methods defined within
    //       our class.

    private double width;
    private double height;
    private double depth;

    // Declare class methods

    void setWidth(double w) {
        width = w;
    }

    void setHeight(double h) {
        height = h;
    }

    void setDepth(double d) {
        depth = d;
    }

    double getVolume() {
        return (width * height * depth);
    }

}

// Main class

class ClassExample07 {

    public static void main(String args[]) {

        // Initialize Variable

        Box myBox1 = new Box();
        Box myBox2 = new Box();

        // Set values

        myBox1.setWidth(10);
        myBox1.setHeight(20);
        myBox1.setDepth(15);

        myBox2.setWidth(3);
        myBox2.setHeight(6);
        myBox2.setDepth(9);

        // Calculate and print volume of the boxes

        System.out.println("Volume of box 1 is " + myBox1.getVolume());
        System.out.println("Volume of box 2 is " + myBox2.getVolume());

    }
}

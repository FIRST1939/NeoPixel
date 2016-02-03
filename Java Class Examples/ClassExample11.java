/*
 * ClassExample11: Overloading constructors
 *
 * This example builds on ClassExample10 and illustrates how constructors can be overloaded.
 *
 * Each example in this series, ClassExample01 thorugh ClassExample12, is meant to show one or two additional
 * concepts that move from the most basic example of a class (ClassExample01) to a more typical class that
 * one would normally expect.
 */

// Define 'Box' class of objects

class Box {

    // Declare class variables

    private double width;
    private double height;
    private double depth;

    // Declare constructors (overloading)

    Box() {
        setDimensions(0, 0, 0);
    }

    Box(double dimension) {
        setDimensions(dimension, dimension, dimension);
    }

    Box(double width, double height, double depth) {
        setDimensions(width, height, depth);
    }

    // Declare class methods

    void setWidth(double width) {
        this.width = width;
    }

    void setHeight(double height) {
        this.height = height;
    }

    void setDepth(double depth) {
        this.depth = depth;
    }

    void setDimensions(double width, double height, double depth) {
        setWidth(width);
        setHeight(height);
        setDepth(depth);
    }

    double getVolume() {
        return (width * height * depth);
    }

}

// Main class

class ClassExample11 {

    public static void main(String args[]) {

        // Initialize Variables

        Box myEmptyBox = new Box();
        Box myCube     = new Box(10);
        Box myBox      = new Box(10, 20, 15);

        // Print volume of the boxes

        System.out.println("Volume of box is  " + myBox.getVolume());
        System.out.println("Volume of cube is " + myCube.getVolume());
        System.out.println("Volume of an empty box is " + myEmptyBox.getVolume());

    }
}

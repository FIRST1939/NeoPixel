/*
 * ClassExample09: Using 'this' to avoid name colissions
 *
 * This example builds on ClassExample08 and illustrates how constructors can be overloaded.
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

class ClassExample09 {

    public static void main(String args[]) {

        // Initialize Variable

        Box myBox1 = new Box();
        Box myBox2 = new Box();

        // Set values

        myBox1.setDimensions(10, 20, 15);
        myBox2.setDimensions(3, 6, 9);

        // Calculate and print volume of the boxes

        System.out.println("Volume of box 1 is " + myBox1.getVolume());
        System.out.println("Volume of box 2 is " + myBox2.getVolume());

    }
}

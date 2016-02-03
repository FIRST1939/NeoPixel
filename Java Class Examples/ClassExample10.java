/*
 * ClassExample10: Constructors
 *
 * This example builds on ClassExample09 and illustrates how constructors can be overloaded.
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

    // Declare constructor

    Box(double width, double height, double depth) {
        System.out.println("Constructing a box");
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

    double getSurfaceArea() {
        double topNbottom = 2 * (depth * width);
        double leftNright = 2 * (depth * height);
        double frontNback = 2 * (width * height);
        return (topNbottom + leftNright + frontNback);
    }

    void cutNhalf() {
        width = (width / 2);
    }

}

// Main class

class ClassExample10 {

    public static void main(String args[]) {

        // Initialize Variables

        Box myBox1 = new Box(10, 20, 15);
        Box myBox2 = new Box( 3,  6,  9);
        Box myBox3 = new Box( 1,  2,  3);

        // Print volume of the boxes

        System.out.println("Volume of box 1 is " + myBox1.getVolume());
        System.out.println("Volume of box 2 is " + myBox2.getVolume());
        System.out.println("Volume of box 3 is " + myBox3.getVolume());

        System.out.println("Surface Area of box 1 is " + myBox1.getSurfaceArea());
        System.out.println("Surface Area of box 2 is " + myBox2.getSurfaceArea());
        System.out.println("Surface Area of box 3 is " + myBox3.getSurfaceArea());

        System.out.println("Cutting in half");
        myBox3.cutNhalf();

        System.out.println("Volume of box 3 is " + myBox3.getVolume());
        System.out.println("Surface Area of box 3 is " + myBox3.getSurfaceArea());


    }
}

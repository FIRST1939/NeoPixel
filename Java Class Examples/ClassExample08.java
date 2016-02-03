/*
 * ClassExample08: Things should only be done once ("DRY" - Don't Repeat Yourself)
 *
 * This example builds on ClassExample07 and illustrates how constructors can be overloaded.
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
    //
    // NOTE: Now that we have a method to set all three dimensions at once, we no longer
    //       need to expose (make 'public') our individual attribute setters.  And, really
    //       we could just get rid of them. However, it is sometimes good to keep them around
    //       for use within the class itself.  Why?  Because we might want to put code in
    //       there that makes sure that the values are valid.  For example, perhaps we eventually
    //       want to make sure we don't have a box whose dimensions are negative in value. We
    //       aren't going to do this now.  But, we could and most 'real' classes would
    //       have them.

    private void setWidth(double w) {
        width = w;
    }

    private void setHeight(double h) {
        height = h;
    }

    private void setDepth(double d) {
        depth = d;
    }

    public void setDimensions(double w, double h, double d) {
        width  = w;
        height = h;
        depth  = d;
    }

    public double getVolume() {
        return (width * height * depth);
    }

}

// Main class

class ClassExample08 {

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

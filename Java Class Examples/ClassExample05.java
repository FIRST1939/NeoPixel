/*
 * ClassExample05: Methods can return values. And, there is a difference between data objects and interaction objects (MVC)
 *
 * This example builds on ClassExample04 and illustrates how constructors can be overloaded.
 *
 * Each example in this series, ClassExample01 thorugh ClassExample12, is meant to show one or two additional
 * concepts that move from the most basic example of a class (ClassExample01) to a more typical class that
 * one would normally expect.
 *
 * However, it is generally bad practice to build into our box program code that interacts with the user.
 * Why?  Because 'Box' is a model of a physical box.  In fact, in a programming paradigm called "MVC" or
 * "Model, View, Controller" we have separate objects for the models (or data objects), the views of the
 * models (the things the human sees), and acting upon controls in the views.  This is an extreamly over
 * simplified view of MVC so don't get too hung up on these definitions yet.  The point is one of
 * "separation of concerns".  We don't want the box itself worrying about what it needs to say to the
 * world.  All it needs to do is calculate its own volume.  So, we have the method return its volume
 * to the caller that asks for it and let the caller then decide what to do with that information.
 */

// Define 'Box' class of objects

class Box {

    // Declare class variables

    double width;
    double height;
    double depth;

    // Declare class methods

    double volume() {
        double vol;

        vol = width * height * depth;

        return vol;   // This is what we use to return a value to the part
                      // of the program that called this method.
    }

}

// Main class

class ClassExample05 {

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

        // NOTE: Now our main program is dealing with how to present the information to
        //       our user.  And, 'box' is simply telling us it's volume.

        System.out.println("Volume of box 1 is " + myBox1.volume());
        System.out.println("Volume of box 2 is " + myBox2.volume());

    }
}

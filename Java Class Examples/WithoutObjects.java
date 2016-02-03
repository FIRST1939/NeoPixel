/*
 * An example of how hellish it is to create three boxes without using classes.
 *
 * NOTE: Arrays could be used to simplify this, but it is still not fun.
 */

class WithoutObjects {

    public static void main(String args[]) {

        // Initialize Variable

        double width1;
        double height1;
        double depth1;
        double vol1;

        double width2;
        double height2;
        double depth2;
        double vol2;

        double width3;
        double height3;
        double depth3;
        double vol3;

        // Set values

        width1  = 10;
        height1 = 20;
        depth1  = 15;

        width2  = 10;
        height2 = 2;
        depth2  = 5;

        width3  = 1;
        height3 = 2;
        depth3  = 1;

        // Calculate volume

        vol1 = width1 * height1 * depth1;
        vol2 = width2 * height2 * depth2;
        vol3 = width3 * height3 * depth3;

        // Print results

        System.out.println("Volume is " + vol1);
        System.out.println("Volume is " + vol2);
        System.out.println("Volume is " + vol3);
    }
}

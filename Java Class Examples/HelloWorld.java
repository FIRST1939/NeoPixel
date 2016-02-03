/*
 * A simple "Hello World" style program used to demonstrate the basics of control structures
 */


class HelloWorld {


    public static void main(String args[]) {

        for(int counter = 0; counter < 3; counter++) {
            System.out.println("hello there! " + counter);
            switch (counter) {
                case 0:
                case 1:
                    System.out.println("Second time");
                    break;
                case 2:
                    System.out.println("Third time");
                    break;
                default:
                    System.out.println("I don't know what to do");
            }
        }

        System.out.println("we are done");

    }
}

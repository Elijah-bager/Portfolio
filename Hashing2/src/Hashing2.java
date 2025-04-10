import components.simplereader.SimpleReader;
import components.simplereader.SimpleReader1L;
import components.simplewriter.SimpleWriter;
import components.simplewriter.SimpleWriter1L;

/**
 * Put a short phrase describing the program here.
 *
 * @author Put your name here
 *
 */
public final class Hashing2 {

    /**
     * No argument constructor--private to prevent instantiation.
     */
    private Hashing2() {
    }

    /**
     * Put a short phrase describing the static method myMethod here.
     */
    /**
     * Simple class representing a 7-digit phone number in the form "XXX-XXXX"
     * for a phone in the immediate OSU area.
     */
    public class PhoneNumber {

        /**
         * The phone number representation.
         */
        private String rep;

        /**
         * Constructor. {@code pNum} must be in the form "XXX-XXXX" where each
         * "X" is a digit '0'-'9'.
         */
        public PhoneNumber(String pNum) {

            this.rep = pNum;
        }

        @Override
        public int hashCode() {
            int sum = 0;
            for (int i = 0; i < this.rep.length(); i++) {
                if (this.rep.charAt(i) != '-') {
                    sum += Character.digit(this.rep.charAt(i), i);
                    System.out.println(sum);

                }
            }
            return sum;

        }

    }

    /**
     * Main method.
     *
     * @param args
     *            the command line arguments
     */
    public static void main(String[] args) {
        SimpleReader in = new SimpleReader1L();
        SimpleWriter out = new SimpleWriter1L();
        /*
         * Put your main program code here; it may call myMethod as shown
         */

        /*
         * Close input and output streams
         */
        in.close();
        out.close();
    }

}

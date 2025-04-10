import components.queue.Queue;
import components.simplereader.SimpleReader;
import components.simplereader.SimpleReader1L;
import components.simplewriter.SimpleWriter;
import components.simplewriter.SimpleWriter1L;
import components.statement.Statement;
import components.statement.Statement1;
import components.utilities.Reporter;
import components.utilities.Tokenizer;

/**
 * Layered implementation of secondary methods {@code parse} and
 * {@code parseBlock} for {@code Statement}.
 *
 * @author Elijah Baugher, Cole Curtis
 *
 */
public final class Statement1Parse1 extends Statement1 {

    /*
     * Private members --------------------------------------------------------
     */

    /**
     * Converts {@code c} into the corresponding {@code Condition}.
     *
     * @param c
     *            the condition to convert
     * @return the {@code Condition} corresponding to {@code c}
     * @requires [c is a condition string]
     * @ensures parseCondition = [Condition corresponding to c]
     */
    private static Condition parseCondition(String c) {
        assert c != null : "Violation of: c is not null";
        assert Tokenizer.isCondition(c) : "Violation of: c is a condition string";
        return Condition.valueOf(c.replace('-', '_').toUpperCase());
    }

    /**
     * Parses an IF or IF_ELSE statement from {@code tokens} into {@code s}.
     *
     * @param tokens
     *            the input tokens
     * @param s
     *            the parsed statement
     * @replaces s
     * @updates tokens
     * @requires <pre>
     * [<"IF"> is a prefix of tokens]  and
     *  [<Tokenizer.END_OF_INPUT> is a suffix of tokens]
     * </pre>
     * @ensures <pre>
     * if [an if string is a proper prefix of #tokens] then
     *  s = [IF or IF_ELSE Statement corresponding to if string at start of #tokens]  and
     *  #tokens = [if string at start of #tokens] * tokens
     * else
     *  [reports an appropriate error message to the console and terminates client]
     * </pre>
     */
    private static void parseIf(Queue<String> tokens, Statement s) {
        assert tokens != null : "Violation of: tokens is not null";
        assert s != null : "Violation of: s is not null";
        assert tokens.length() > 0 && tokens.front().equals("IF")
                : "" + "Violation of: <\"IF\"> is proper prefix of tokens";
        tokens.dequeue();
        //Checks if next string is condition
        Reporter.assertElseFatalError(
                tokens != null && Tokenizer.isCondition(tokens.front()),
                "TOKENS IS NOT A CONDITION");
        Statement nsIf = s.newInstance();
        String condition = tokens.dequeue();
        //Checks if next string is THEN from "IF TRUE THEN"
        Reporter.assertElseFatalError(tokens != null && tokens.front().equals("THEN"),
                "THERE IS NO THEN");
        // Parses condition into c
        Condition c = parseCondition(condition);
        tokens.dequeue();
        //Check is tokens is empty
        Reporter.assertElseFatalError(tokens != null, "QUEUE IS NULL");
        //Parses if block into nsIF
        nsIf.parseBlock(tokens);
        //Checks if front is an end statement or else
        Reporter.assertElseFatalError(
                tokens.front().equals("END") || tokens.front().equals("ELSE"),
                "NO END OR ELSE");
        //If statement for else block
        if (tokens.front().equals("ELSE")) {

            Statement nsElse = s.newInstance();
            tokens.dequeue();
            //Checks if tokens is empty
            Reporter.assertElseFatalError(tokens != null, "QUEUE IS NULL");
            //Parses else block into nsElse
            nsElse.parseBlock(tokens);
            String end1 = tokens.dequeue();
            String end2 = tokens.dequeue();
            Reporter.assertElseFatalError(
                    tokens != null && end1.equals("END") && end2.equals("IF"),
                    "NOT AN IF STATEMENT");
            //Assemble ifElse
            s.assembleIfElse(c, nsIf, nsElse);
        } else {
            tokens.dequeue();

            String end2 = tokens.dequeue();
            Reporter.assertElseFatalError(tokens != null && end2.equals("IF"),
                    "NOT AN IF STATEMENT");
            //Assemble if
            s.assembleIf(c, nsIf);
        }

    }

    /**
     * Parses a WHILE statement from {@code tokens} into {@code s}.
     *
     * @param tokens
     *            the input tokens
     * @param s
     *            the parsed statement
     * @replaces s
     * @updates tokens
     * @requires <pre>
     * [<"WHILE"> is a prefix of tokens]  and
     *  [<Tokenizer.END_OF_INPUT> is a suffix of tokens]
     * </pre>
     * @ensures <pre>
     * if [a while string is a proper prefix of #tokens] then
     *  s = [WHILE Statement corresponding to while string at start of #tokens]  and
     *  #tokens = [while string at start of #tokens] * tokens
     * else
     *  [reports an appropriate error message to the console and terminates client]
     * </pre>
     */
    private static void parseWhile(Queue<String> tokens, Statement s) {
        assert tokens != null : "Violation of: tokens is not null";
        assert s != null : "Violation of: s is not null";
        assert tokens.length() > 0 && tokens.front().equals("WHILE")
                : "" + "Violation of: <\"WHILE\"> is proper prefix of tokens";

        tokens.dequeue();
        //Checks if next string is condition
        Reporter.assertElseFatalError(
                tokens != null && Tokenizer.isCondition(tokens.front()),
                "TOKENS IS NOT A CONDITION");
        Statement ns = s.newInstance();
        String condition = tokens.dequeue();
        // Parses condition into c
        Condition c = parseCondition(condition);
        //Checks if next string is DO from "WHILE TRUE DO"
        Reporter.assertElseFatalError(tokens != null && tokens.front().equals("DO"),
                "THERE IS NO DO");
        tokens.dequeue();
        //Checks if queue is empty after dequeue
        Reporter.assertElseFatalError(tokens != null, "QUEUE IS NULL");

        //parses block of statements into ns
        ns.parseBlock(tokens);
        String end1 = tokens.dequeue();
        String end2 = tokens.dequeue();
        //Checks to see if the end statements in while loop are in tokens
        Reporter.assertElseFatalError(
                tokens != null && end1.equals("END") && end2.equals("WHILE"),
                "NOT A WHILE STATEMENT");
        s.assembleWhile(c, ns);

    }

    /**
     * Parses a CALL statement from {@code tokens} into {@code s}.
     *
     * @param tokens
     *            the input tokens
     * @param s
     *            the parsed statement
     * @replaces s
     * @updates tokens
     * @requires [identifier string is a proper prefix of tokens]
     * @ensures <pre>
     * s =
     *   [CALL Statement corresponding to identifier string at start of #tokens]  and
     *  #tokens = [identifier string at start of #tokens] * tokens
     * </pre>
     */
    private static void parseCall(Queue<String> tokens, Statement s) {
        assert tokens != null : "Violation of: tokens is not null";
        assert s != null : "Violation of: s is not null";
        assert tokens.length() > 0 && Tokenizer.isIdentifier(tokens.front())
                : "" + "Violation of: identifier string is proper prefix of tokens";
        //Assembles call with instruction
        String call = tokens.dequeue();
        s.assembleCall(call);

    }

    /*
     * Constructors -----------------------------------------------------------
     */

    /**
     * No-argument constructor.
     */
    public Statement1Parse1() {
        super();
    }

    /*
     * Public methods ---------------------------------------------------------
     */

    @Override
    public void parse(Queue<String> tokens) {
        assert tokens != null : "Violation of: tokens is not null";
        assert tokens.length() > 0
                : "" + "Violation of: Tokenizer.END_OF_INPUT is a suffix of tokens";
        //Clears this so it can be replaced
        this.clear();
        //If else statement to parse individual statements
        String front = tokens.front();
        if (front.equals("IF")) {
            parseIf(tokens, this);
        } else if (front.equals("WHILE")) {
            parseWhile(tokens, this);
        } else if (Tokenizer.isIdentifier(front)) {
            parseCall(tokens, this);
        } else {
            //Ends program if there is no identifier
            Reporter.fatalErrorToConsole("THERE IS NO STATEMENT");
        }

    }

    @Override
    public void parseBlock(Queue<String> tokens) {
        assert tokens != null : "Violation of: tokens is not null";
        assert tokens.length() > 0
                : "" + "Violation of: Tokenizer.END_OF_INPUT is a suffix of tokens";
        //Clears this so it can be replaced
        this.clear();
        String front = tokens.front();
        int i = 0;
        //While loop to add series of parsed statements to this
        while (Tokenizer.isIdentifier(front) || front.equals("WHILE")
                || front.equals("IF")) {
            Statement ns = this.newInstance();
            ns.parse(tokens);
            this.addToBlock(i, ns);
            front = tokens.front();
            i++;
        }

    }

    /*
     * Main test method -------------------------------------------------------
     */

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
         * Get input file name
         */
        out.print("Enter valid BL statement(s) file name: ");
        String fileName = in.nextLine();
        /*
         * Parse input file
         */
        out.println("*** Parsing input file ***");
        Statement s = new Statement1Parse1();
        SimpleReader file = new SimpleReader1L(fileName);
        Queue<String> tokens = Tokenizer.tokens(file);
        file.close();
        s.parse(tokens); // replace with parseBlock to test other method
        /*
         * Pretty print the statement(s)
         */
        out.println("*** Pretty print of parsed statement(s) ***");
        s.prettyPrint(out, 0);

        in.close();
        out.close();
    }

}

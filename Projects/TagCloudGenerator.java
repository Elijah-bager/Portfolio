
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.Scanner;
import java.util.Set;

/**
 * Program to receive input file and generate an HTML file in which to display a
 * tag cloud
 *
 * @author Elijah Baugher
 *
 */
public final class TagCloudGenerator {

    /**
     * No argument constructor--private to prevent instantiation.
     */
    private TagCloudGenerator() {
    }

    /**
     * Comparator to sort values in decreasing order.
     */
    private static class numberSort
            implements Comparator<java.util.Map.Entry<String, Integer>> {
        @Override
        public int compare(java.util.Map.Entry<String, Integer> o1,
                java.util.Map.Entry<String, Integer> o2) {
            int compare = o2.getValue().compareTo(o1.getValue());
            if (compare == 0) {
                compare = o1.getKey().compareTo(o2.getKey());
            }
            return compare;
        }
    }

    /**
     * Comparator to sort words in alphabetical order.
     */
    private static class wordSort
            implements Comparator<java.util.Map.Entry<String, Integer>> {
        @Override
        public int compare(java.util.Map.Entry<String, Integer> o1,
                java.util.Map.Entry<String, Integer> o2) {
            int compare = o1.getKey().compareToIgnoreCase(o2.getKey());
            if (compare == 0) {
                compare = o2.getValue().compareTo(o1.getValue());
            }
            return compare;

        }

    }

    /**
     * Generates the set of characters in the given {@code String} into the
     * given {@code Set}.
     *
     * @param str
     *            the given {@code String}
     *
     * @param charSet
     *            the {@code Set} to be replaced
     *
     * @replaces charSet
     *
     * @ensures charSet = entries(str)
     */
    private static void generateElements(String str, Set<Character> charSet) {
        assert str != null : "Violation of: str is not null";
        assert charSet != null : "Violation of: charSet is not null";

        int i = 0;

        char character = 'a';

        charSet.clear();

        while (i < str.length()) {

            if (!charSet.contains(str.charAt(i))) {

                character = str.charAt(i);

                charSet.add(character);
            }

            i++;
        }
    }

    /**
     * Returns the first "word" (maximal length string of characters not in
     * {@code separators}) or "separator string" (maximal length string of
     * characters in {@code separators}) in the given {@code text} starting at
     * the given {@code position}.
     *
     * @param text
     *            the {@code String} from which to get the word or separator
     *            string
     * @param position
     *            the starting index
     * @param separators
     *            the {@code Set} of separator characters
     * @return the first word or separator string found in {@code text} starting
     *         at index {@code position}
     * @requires 0 <= position < |text|
     * @ensures <pre>
     * nextWordOrSeparator =
     *   text[position, position + |nextWordOrSeparator|)  and
     * if entries(text[position, position + 1)) intersection separators = {}
     * then
     *   entries(nextWordOrSeparator) intersection separators = {}  and
     *   (position + |nextWordOrSeparator| = |text|  or
     *    entries(text[position, position + |nextWordOrSeparator| + 1))
     *      intersection separators /= {})
     * else
     *   entries(nextWordOrSeparator) is subset of separators  and
     *   (position + |nextWordOrSeparator| = |text|  or
     *    entries(text[position, position + |nextWordOrSeparator| + 1))
     *      is not subset of separators)
     * </pre>
     */
    private static String nextWordOrSeparator(String text, int position,
            HashSet<Character> separators) {
        assert text != null : "Violation of: text is not null";
        assert separators != null : "Violation of: separators is not null";
        assert 0 <= position : "Violation of: 0 <= position";
        assert position < text.length() : "Violation of: position < |text|";
        int count = position;
        boolean contains = separators.contains(text.charAt(position));

        while (count < text.length()
                && separators.contains(text.charAt(count)) == contains) {

            count++;
        }
        return text.substring(position, count);

    }

    /**
     * Uses the inputs from {@code file} and puts them into {@code ogMap} and
     * {@code ogSequence}. Generates the words "String" and the counts "Integer"
     * into the {@code ogMap}.
     *
     *
     * @param ogMap
     *            The {@code Map} that will receive the word and count inputs
     *
     *
     * @param file
     *            The {@code SimpleReader} from which the Map and Sequence will
     *            get their inputs
     *
     * @replaces {@code ogMap}
     *
     *
     * @ensures <pre> {@code ogMap}= elements(file) </pre>
     */
    private static void wordandCounter(Map<String, Integer> ogMap, String file)
            throws IOException {

        String seperatorString = " \t\n\r,-.!?[]';:/()*";

        HashSet<Character> seperatorSet = new HashSet<>();

        // Assigns the separators to seperatorSet

        generateElements(seperatorString, seperatorSet);
        int mcount = 0;

        String mword;
        for (int i = 0; i < file.length(); i++) {
            mcount = 0;

            //Finds first word or separator

            mword = nextWordOrSeparator(file, 0, seperatorSet).toLowerCase();

            //Creates new text that excludes the found word

            file = file.substring(mword.length(), file.length());
            //if statement to make sure the word is not a separator
            if (!seperatorSet.contains(mword.charAt(0))) {
                if (ogMap.containsKey(mword)) {

                    /*
                     * if the map has key word, it is removed and then added
                     * back with one higher count
                     */

                    mcount = ogMap.get(mword);

                    ogMap.put(mword, mcount + 1);

                } else {
                    mcount = 1;

                    ogMap.put(mword, mcount);
                }

            }

        }
    }

    /**
     *
     * @param collectedPairs
     * @param wordSort
     * @return x
     */
    public static PriorityQueue<java.util.Map.Entry<String, Integer>> wordSorter(
            Map<String, Integer> collectedPairs,
            Comparator<java.util.Map.Entry<String, Integer>> wordSort) {
        Set<java.util.Map.Entry<String, Integer>> n = collectedPairs.entrySet();
        PriorityQueue<java.util.Map.Entry<String, Integer>> pq = new PriorityQueue<>(
                wordSort);
        for (java.util.Map.Entry<String, Integer> e : n) {
            pq.add(e);
        }
        return pq;

    }

    /**
     *
     * @param collectedPairs
     * @param countSort
     *
     * @return x
     */
    public static PriorityQueue<java.util.Map.Entry<String, Integer>> collectedPairs,
    Comparator<java.util.Map.Entry<String,Integer>>wordSort,
    int num)
    {
        int count = 0;
        while (count < this.num) {
            pq.add(this.collectedPairs.poll());
            count++;
        }
        return pq;
    }

    /**
     * Creates an HTML page that displays a tag cloud of the presented words and
     * counts.
     *
     * @param out
     *            The {@code out.conent} the generates the HTML file
     *
     * @param sm
     *            The {@code SortingMachine} containing and displaying the
     *            "String" terms and the "Integer" counts
     *
     * @param n
     *            The {@code Integer} holding the number of values wished to be
     *            displayed in tag cloud
     *
     *
     * @param text
     *            The {@code String} used to display the output file name
     *
     * @param min
     *            The {@code Integer} that holds the minimum word count
     *
     * @param max
     *            The {@code Integer} that holds the maximum word count
     *
     * @updates {@code out.content}
     *
     * @ensures <pre> {@code out.content= #out.content * [HTML Tags]} </pre>
     */

    private static void outputIndex(PrintWriter out, String name,
            PriorityQueue<Map.Entry<String, Integer>> holder, int n, int min, int max) {

        out.println("<html>");
        out.println("<head>");
        out.println("<title> Tag cloud </title>");
        out.println("<link href=\"https://cse22x1.engineering.osu.edu/2231/"
                + "web-sw2/assignments/projects/tag-cloud-generator/data/tagcloud.css\" rel=\"stylesheet\" type=\"text/css\">\r\n"
                + "<link href=\"tagcloud.css\" rel=\"stylesheet\" type=\"text/css\">");
        out.println("</head>");
        out.println("<body>");
        out.println("<hr>");
        out.println(
                "<h1 style='color:Tomato'> Top " + n + " words in " + name + " </h1>");
        out.println("<hr>");
        out.println("<div class=\"cdiv\">");
        out.println("<p class=\"cbox\">");

        int smallest = 11;
        int largest = 48;
        int f = 0;

        for (Map.Entry<String, Integer> m : holder) {
            if (max != min) {
                f = (m.getValue() - min) * largest / (max - min) + smallest;
            } else {
                f = smallest;
            }
            if (f > largest) {
                f = largest;
            }

            out.println(
                    "<span style=\"cursor:default\" class=\"f" + f + " title=\"count: "
                            + m.getValue() + "\">" + m.getKey() + " </span>");
        }

        out.println("</p>");
        out.println("</div>");
        out.println("</body>");
        out.println("</html>");

    }

    /**
     * Main method.
     *
     * @param args
     *            the command line arguments
     */
    public static void main(String[] args) {
        /*
         * Open input and output streams
         */

        Scanner scanner = new Scanner(System.in);
        System.out.println("Enter input file>>");
        String input = scanner.nextLine();
        System.out.println("Enter output file>>");
        String output = scanner.nextLine();
        BufferedReader in;
        PrintWriter out = null;

        try {

            in = new BufferedReader(new FileReader(input));

        } catch (IOException e) {
            System.err.println("FILE CANNOT OPEN");
            scanner.close();
            return;

        }
        try {

            out = new PrintWriter(new BufferedWriter(new FileWriter(output)));
        } catch (IOException e) {
            System.err.println("NO OUTPUT FILE");

        }
        try {

            if (out != null) {
                System.out.println("Enter Number of words for tag cloud");
                int n = scanner.nextInt();
                String text = in.readLine();

                Map<String, Integer> map = new HashMap<>();
                while (in.readLine() != null) {

                    wordandCounter(map, text);
                    text = in.readLine();

                }

                // Create comparator to sort in order of decreasing word count
                Comparator<Map.Entry<String, Integer>> values = new numberSort();
                // Sort words in decreasing order
                PriorityQueue<Map.Entry<String, Integer>> sortedWords = wordSorter(map,
                        values);

                // Create a comparator to sort in alphabetical order
                Comparator<Map.Entry<String, Integer>> words = new wordSort();
                PriorityQueue<Map.Entry<String, Integer>> sortedValues = new PriorityQueue<>(
                        map.size(), words);

                int min = 0;
                int max = 0;

                for (Map.Entry<String, Integer> m : sortedWords) {

                    if (m.getValue() > max) {
                        max = m.getValue();
                        min = max;
                    }
                    if (m.getValue() <= min) {
                        min = m.getValue();
                    }

                }

                outputIndex(out, input, sortedWords, n, min, max);

            }

        } catch (IOException e) {
            System.err.println("WRONG");

        }
        try {
            scanner.close();
            in.close();
            out.close();
        } catch (IOException e) {
            System.err.println("Cannot close files");
        }

    }

}

import components.binarytree.BinaryTree;
import components.binarytree.BinaryTree1;
import components.simplereader.SimpleReader;
import components.simplereader.SimpleReader1L;
import components.simplewriter.SimpleWriter;
import components.simplewriter.SimpleWriter1L;

/**
 * Utility class with implementation of binary search tree static, generic
 * methods isInTree (and removeSmallest).
 *
 * @mathdefinitions <pre>
 * IS_BST(
 *   tree: binary tree of T
 *  ): boolean satisfies
 *  [tree satisfies the binary search tree properties as described in the
 *   slides with the ordering reported by compareTo for T, including that
 *   it has no duplicate labels]
 * </pre>
 *
 * @author Elijah Baugher
 *
 */
public final class BinarySearchTreeMethods {

    /**
     * Private constructor so this utility class cannot be instantiated.
     */
    private BinarySearchTreeMethods() {
    }

    /**
     * Returns the {@code String} prefix representation of the given
     * {@code BinaryTree<T>}.
     *
     * @param <T>
     *            the type of the {@code BinaryTree} node labels
     * @param t
     *            the {@code BinaryTree} to convert to a {@code String}
     * @return the prefix representation of {@code t}
     * @ensures treeToString = [the String prefix representation of t]
     */
    public static <T> String treeToString(BinaryTree<T> t) {
        BinaryTree<T> lt = new BinaryTree1<>();
        BinaryTree<T> rt = new BinaryTree1<>();
        String quote = "";
        if (t.size() != 0) {
            T root = t.disassemble(lt, rt);
            quote = "(" + root + ")" + "(" + treeToString(lt) + ")" + "("
                    + treeToString(rt) + ")";
            t.assemble(root, lt, rt);
        } else {

            quote = "(" + ")";
        }
        return quote;

    }

    /**
     * Returns a copy of the the given {@code BinaryTree}.
     *
     * @param t
     *            the {@code BinaryTree} to copy
     * @return a copy of the given {@code BinaryTree}
     * @ensures copy = t
     */
    public static BinaryTree<Integer> copy(BinaryTree<Integer> t) {
        BinaryTree<Integer> lt = new BinaryTree1<>();
        BinaryTree<Integer> rt = new BinaryTree1<>();
        BinaryTree<Integer> copy = new BinaryTree1<>();

        if (t.size() != 0) {

            int root = t.disassemble(lt, rt);

            copy.assemble(root, copy(lt), copy(rt));

            t.assemble(root, lt, rt);
        }
        return copy;

    }

    /**
     * Returns whether {@code x} is in {@code t}.
     *
     * @param <T>
     *            type of {@code BinaryTree} labels
     * @param t
     *            the {@code BinaryTree} to be searched
     * @param x
     *            the label to be searched for
     * @return true if t contains x, false otherwise
     * @requires IS_BST(t)
     * @ensures isInTree = (x is in labels(t))
     */
    public static <T extends Comparable<T>> boolean isInTree(BinaryTree<T> t, T x) {

        BinaryTree<T> lt = new BinaryTree1<>();
        BinaryTree<T> rt = new BinaryTree1<>();
        boolean inTree = false;
        if (!(t.height() == 0)) {
            T root = t.disassemble(lt, rt);
            int compare = x.compareTo(root);
            if (compare > 0) {
                inTree = isInTree(rt, x);
            } else if (compare < 0) {
                inTree = isInTree(lt, x);
            } else if (compare == 0) {
                inTree = true;
            }
            t.assemble(root, lt, rt);

        }

        return inTree;

    }

    /**
     * Removes and returns the smallest (left-most) label in {@code t}.
     *
     * @param <T>
     *            type of {@code BinaryTree} labels
     * @param t
     *            the {@code BinaryTree} from which to remove the label
     * @return the smallest label in the given {@code BinaryTree}
     * @updates t
     * @requires IS_BST(t) and |t| > 0
     * @ensures <pre>
     * IS_BST(t)  and  removeSmallest = [the smallest label in #t]  and
     *  labels(t) = labels(#t) \ {removeSmallest}
     * </pre>
     */
    public static <T> T removeSmallest(BinaryTree<T> t) {
        BinaryTree<T> lt = new BinaryTree1<>();
        BinaryTree<T> rt = new BinaryTree1<>();
        T x = null;
        if (t.height() != 0) {
            T root = t.disassemble(lt, rt);
            x = removeSmallest(lt);
            t.assemble(root, lt, rt);
        } else {
            x = lt.root();
        }

        return x;
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
         * Input tree labels and construct BST.
         */
        out.println("Input the distinct labels for a binary search tree "
                + "in the order in which you want them inserted.");
        out.println("Press Enter on an empty line to terminate your input.");
        out.println();
        out.print("Next label: ");
        String str = in.nextLine();
        BinaryTree<String> t = new BinaryTree1<String>();
        while (str.length() > 0) {
            BinaryTreeUtility.insertInTree(t, str);
            out.println();
            out.println("t = " + BinaryTreeUtility.treeToString(t));
            out.println();
            out.print("Next label: ");
            str = in.nextLine();
        }
        /*
         * Input strings and check whether each is in the BST or not.
         */
        out.println();
        out.print("  Input a label to search "
                + "(or just press Enter to input a new tree): ");
        String label = in.nextLine();
        while (label.length() > 0) {
            if (isInTree(t, label)) {
                out.println("    \"" + label + "\" is in the tree");
            } else {
                out.println("    \"" + label + "\" is not in the tree");
            }
            out.print("  Input a label to search "
                    + "(or just press Enter to terminate the program): ");
            label = in.nextLine();
        }
        /*
         * Output BST labels in order.
         */
//        out.println();
//        out.println("Labels in BST in order:");
//        while (t.size() > 0) {
//            label = removeSmallest(t);
//            out.println("  " + label);
//        }

        in.close();
        out.close();
    }
}

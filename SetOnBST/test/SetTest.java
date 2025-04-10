import static org.junit.Assert.assertEquals;

import org.junit.Test;

import components.set.Set;

/**
 * JUnit test fixture for {@code Set<String>}'s constructor and kernel methods.
 *
 * @author Elijah Baugher, Cole Curtis
 *
 */
public abstract class SetTest {

    /**
     * Invokes the appropriate {@code Set} constructor for the implementation
     * under test and returns the result.
     *
     * @return the new set
     * @ensures constructorTest = {}
     */
    protected abstract Set<String> constructorTest();

    /**
     * Invokes the appropriate {@code Set} constructor for the reference
     * implementation and returns the result.
     *
     * @return the new set
     * @ensures constructorRef = {}
     */
    protected abstract Set<String> constructorRef();

    /**
     * Creates and returns a {@code Set<String>} of the implementation under
     * test type with the given entries.
     *
     * @param args
     *            the entries for the set
     * @return the constructed set
     * @requires [every entry in args is unique]
     * @ensures createFromArgsTest = [entries in args]
     */
    private Set<String> createFromArgsTest(String... args) {
        Set<String> set = this.constructorTest();
        for (String s : args) {
            assert !set.contains(s) : "Violation of: every entry in args is unique";
            set.add(s);
        }
        return set;
    }

    /**
     * Creates and returns a {@code Set<String>} of the reference implementation
     * type with the given entries.
     *
     * @param args
     *            the entries for the set
     * @return the constructed set
     * @requires [every entry in args is unique]
     * @ensures createFromArgsRef = [entries in args]
     */
    private Set<String> createFromArgsRef(String... args) {
        Set<String> set = this.constructorRef();
        for (String s : args) {
            assert !set.contains(s) : "Violation of: every entry in args is unique";
            /*
             * Call method under test
             */

            set.add(s);
        }
        return set;
    }

    /*
     * Test for Constructor
     */
    @Test
    public void testConstructorTest() {
        Set<String> s1 = this.constructorRef();
        Set<String> s2 = this.constructorTest();
        assertEquals(s1, s2);
    }

    /*
     * Test for kernel methods
     */

    @Test
    public void addTestFromEmpty() {
        /*
         * Set up variables
         */
        Set<String> s1 = this.createFromArgsTest();
        Set<String> s1Expected = this.createFromArgsRef("apple");
        /*
         * Call method under test
         */

        s1.add("apple");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(s1Expected, s1);
    }

    @Test
    public void addTestNonEmptyOne() {
        /*
         * Set up variables
         */
        Set<String> s1 = this.createFromArgsTest("apple");
        Set<String> s1Expected = this.createFromArgsRef("apple", "orange");
        /*
         * Call method under test
         */

        s1.add("orange");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(s1Expected, s1);
    }

    @Test
    public void addTestNonEmptyTwo() {
        /*
         * Set up variables
         */
        Set<String> s1 = this.createFromArgsTest("apple", "orange");
        Set<String> s1Expected = this.createFromArgsRef("apple", "orange", "banana");
        /*
         * Call method under test
         */

        s1.add("banana");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(s1Expected, s1);
    }

    @Test
    public void removeTestEmpty() {
        /*
         * Set up variables
         */
        Set<String> s1 = this.createFromArgsTest("apple");
        Set<String> s1Expected = this.createFromArgsRef();
        /*
         * Call method under test
         */

        s1.remove("apple");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(s1Expected, s1);
    }

    @Test
    public void removeTestNonEmptyThree() {
        /*
         * Set up variables
         */
        Set<String> s1 = this.createFromArgsTest("apple", "orange", "banana");
        Set<String> s1Expected = this.createFromArgsRef("apple", "banana");
        /*
         * Call method under test
         */

        s1.remove("orange");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(s1Expected, s1);
    }

    @Test
    public void removeTestNonEmptyTwo() {
        /*
         * Set up variables
         */
        Set<String> s1 = this.createFromArgsTest("apple", "banana");
        Set<String> s1Expected = this.createFromArgsRef("apple");
        /*
         * Call method under test
         */

        s1.remove("banana");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(s1Expected, s1);
    }

    @Test
    public void containsTestNonEmptyTrue() {
        /*
         * Set up variables
         */
        Set<String> s1 = this.createFromArgsTest("apple", "banana");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(true, s1.contains("apple"));
    }

    @Test
    public void containsTestNonEmptyFalse() {
        /*
         * Set up variables
         */
        Set<String> s1 = this.createFromArgsTest("apple", "banana");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(false, s1.contains("orange"));
    }

    @Test
    public void containsTestNonEmptyTrue2() {
        /*
         * Set up variables
         */
        Set<String> s1 = this.createFromArgsTest("apple", "banana", "pear");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(true, s1.contains("pear"));
    }

    @Test
    public void sizeTestEmpty() {
        /*
         * Set up variables
         */
        Set<String> s1 = this.createFromArgsTest();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(0, s1.size());
    }

    @Test
    public void sizeTestNonEmptyOne() {
        /*
         * Set up variables
         */
        Set<String> s1 = this.createFromArgsTest("orange");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(1, s1.size());
    }

    @Test
    public void sizeTestNonEmptyTwo() {
        /*
         * Set up variables
         */
        Set<String> s1 = this.createFromArgsTest("apple", "watermelon");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(2, s1.size());
    }

    @Test
    public void sizeTestNonEmptyThree() {
        /*
         * Set up variables
         */
        Set<String> s1 = this.createFromArgsTest("apple", "banana", "pear");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(3, s1.size());
    }

    @Test
    public final void testRemoveAny() {
        /*
         * Set up variables
         */
        Set<String> s1 = this.createFromArgsTest("1", "2", "3");
        Set<String> s2 = this.createFromArgsTest("1", "2", "3");
        boolean setBoo = true;
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(setBoo, s2.contains(s1.removeAny()));

    }
}

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import components.map.Map;

/**
 * JUnit test fixture for {@code Map<String, String>}'s constructor and kernel
 * methods.
 *
 * @author Elijah Baugher, Cole Curtis
 *
 */
public abstract class MapTest {

    /**
     * Invokes the appropriate {@code Map} constructor for the implementation
     * under test and returns the result.
     *
     * @return the new map
     * @ensures constructorTest = {}
     */
    protected abstract Map<String, String> constructorTest();

    /**
     * Invokes the appropriate {@code Map} constructor for the reference
     * implementation and returns the result.
     *
     * @return the new map
     * @ensures constructorRef = {}
     */
    protected abstract Map<String, String> constructorRef();

    /**
     *
     * Creates and returns a {@code Map<String, String>} of the implementation
     * under test type with the given entries.
     *
     * @param args
     *            the (key, value) pairs for the map
     * @return the constructed map
     * @requires <pre>
     * [args.length is even]  and
     * [the 'key' entries in args are unique]
     * </pre>
     * @ensures createFromArgsTest = [pairs in args]
     */
    private Map<String, String> createFromArgsTest(String... args) {
        assert args.length % 2 == 0 : "Violation of: args.length is even";
        Map<String, String> map = this.constructorTest();
        for (int i = 0; i < args.length; i += 2) {
            assert !map.hasKey(args[i])
                    : "" + "Violation of: the 'key' entries in args are unique";
            map.add(args[i], args[i + 1]);
        }
        return map;
    }

    /**
     *
     * Creates and returns a {@code Map<String, String>} of the reference
     * implementation type with the given entries.
     *
     * @param args
     *            the (key, value) pairs for the map
     * @return the constructed map
     * @requires <pre>
     * [args.length is even]  and
     * [the 'key' entries in args are unique]
     * </pre>
     * @ensures createFromArgsRef = [pairs in args]
     */
    private Map<String, String> createFromArgsRef(String... args) {
        assert args.length % 2 == 0 : "Violation of: args.length is even";
        Map<String, String> map = this.constructorRef();
        for (int i = 0; i < args.length; i += 2) {
            assert !map.hasKey(args[i])
                    : "" + "Violation of: the 'key' entries in args are unique";
            map.add(args[i], args[i + 1]);
        }
        return map;
    }

    /*
     * Tests for constructor
     */

    @Test
    public void constructorEmptyTest() {
        Map<String, String> m1 = this.createFromArgsTest();
        Map<String, String> m1Expected = this.createFromArgsRef();
        assertEquals(m1Expected, m1);
    }

    @Test
    public final void testNoArgumentConstructor() {
        /*
         * Set up variables and call method under test
         */
        Map<String, String> m = this.constructorTest();
        Map<String, String> mExpected = this.constructorRef();

        /*
         * Assert that values of variables match expectations
         */
        assertEquals(mExpected, m);
    }
    /*
     * Test cases for kernel methods
     */

    @Test
    public final void testAddFromEmptyWithOne() {
        /*
         * Set up variables
         */
        Map<String, String> m = this.createFromArgsTest("A", "123");
        Map<String, String> mExpected = this.createFromArgsRef();

        /*
         * Call method under test
         */
        mExpected.add("A", "123");

        /*
         * Assert that values of variables match expectations
         */
        assertEquals(mExpected, m);

    }

    @Test
    public final void testAddFromEmptyWithTwo() {
        /*
         * Set up variables
         */
        Map<String, String> m = this.createFromArgsTest("A", "123", "B", "456");
        Map<String, String> mExpected = this.createFromArgsRef();

        /*
         * Call method under test
         */
        mExpected.add("A", "123");
        mExpected.add("B", "456");

        /*
         * Assert that values of variables match expectations
         */
        assertEquals(mExpected, m);
    }

    @Test
    public final void testAddNonEmptyOne() {
        /*
         * Set up variables
         */
        Map<String, String> m = this.createFromArgsTest("A", "123", "B", "456");
        Map<String, String> mExpected = this.createFromArgsRef("A", "123");

        /*
         * Call method under test
         */
        mExpected.add("B", "456");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(mExpected, m);
    }

    @Test
    public final void testRemoveNonEmptyToEmpty() {
        /*
         * Set up variables
         */
        Map<String, String> m = this.createFromArgsTest("Pizza", "1");
        Map<String, String> mExpected = this.createFromArgsRef();

        /*
         * Call method under test
         */
        m.remove("Pizza");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(mExpected, m);

    }

    @Test
    public final void testRemoveNonEmptyOne() {
        /*
         * Set up variables
         */
        Map<String, String> m = this.createFromArgsTest("A", "123", "B", "456");
        Map<String, String> mExpected = this.createFromArgsRef("A", "123");

        /*
         * Call method under test
         */
        m.remove("B");
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(mExpected, m);
    }

    @Test
    public final void testRemoveAny() {
        /*
         * Set up variables
         */
        Map<String, String> m = this.createFromArgsTest();
        Map<String, String> mExpected = this.createFromArgsRef();

        /*
         * Call method under test
         */

        /*
         * Assert that values of variables match expectations
         */

    }
    /*
     * Test for Standard Methods
     */

//Test for new instance
    @Test
    public final void testNewInstance() {
        /*
         * Set up variables
         */
        Map<String, String> m = this.createFromArgsTest("B", "A");

        /*
         * Call method under test
         */
        Map<String, String> x = m.newInstance();
        Map<String, String> z = m.newInstance();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(x, z);
    }

//Test for TransferFrom
    @Test
    public final void testTransferFrom() {
        /*
         * Set up variables
         */

        Map<String, String> n = this.createFromArgsTest("A", "B");
        Map<String, String> x = this.createFromArgsTest();
        Map<String, String> z = this.createFromArgsRef("A", "B");

        /*
         * Call method under test
         */
        x.transferFrom(n);
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(x, z);

    }

//Test for clear
    @Test
    public final void testClear() {
        /*
         * Set up variables
         */
        Map<String, String> n = this.createFromArgsTest("A", "B");
        Map<String, String> x = this.createFromArgsRef();
        /*
         * Call method under test
         */
        n.clear();
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(n, x);
    }

    @Test
    public void valueTest1() {
        /*
         * Set up variables
         */
        Map<String, String> m1 = this.createFromArgsTest("one", "1");
        String key = "one";
        String m1Expected = "1";
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(m1Expected, m1.value(key));
    }

    @Test
    public void valueTest2() {
        /*
         * Set up variables
         */
        Map<String, String> m1 = this.createFromArgsTest("one", "1", "three", "5");
        String key = "three";
        String m1Expected = "5";
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(m1Expected, m1.value(key));
    }

    @Test
    public void valueTest3() {
        /*
         * Set up variables
         */
        Map<String, String> m1 = this.createFromArgsTest("one", "1", "three", "5", "ten",
                "2");
        String key = "ten";
        String m1Expected = "2";
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(m1Expected, m1.value(key));
    }

    @Test
    public void hasKeyTest1() {
        /*
         * Set up variables
         */
        Map<String, String> m1 = this.createFromArgsTest("one", "1");
        String key = "one";
        boolean m1Expected = true;
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(m1Expected, m1.hasKey(key));
    }

    @Test
    public void hasKeyTest2() {
        /*
         * Set up variables
         */
        Map<String, String> m1 = this.createFromArgsTest("one", "1", "apple", "7",
                "tacos", "2");
        String key = "apple";
        boolean m1Expected = true;
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(m1Expected, m1.hasKey(key));
    }

    @Test
    public void hasKeyTest3() {
        /*
         * Set up variables
         */
        Map<String, String> m1 = this.createFromArgsTest("one", "1", "tacos", "4",
                "unicorns", "7", "sheep", "2");
        String key = "oranges";
        boolean m1Expected = false;
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(m1Expected, m1.hasKey(key));
    }

    @Test
    public void sizeZeroTest() {
        /*
         * Set up variables
         */
        Map<String, String> m1 = this.createFromArgsTest();
        int m1Expected = 0;
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(m1Expected, m1.size());
    }

    @Test
    public void sizeOneTest() {
        /*
         * Set up variables
         */
        Map<String, String> m1 = this.createFromArgsTest("One", "1");
        int m1Expected = 1;
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(m1Expected, m1.size());
    }

    @Test
    public void sizeTest1() {
        /*
         * Set up variables
         */
        Map<String, String> m1 = this.createFromArgsTest("one", "1", "two", "2", "three",
                "3", "four", "4");
        int m1Expected = 4;
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(m1Expected, m1.size());
    }

    @Test
    public void sizeTenTest() {
        /*
         * Set up variables
         */
        Map<String, String> m1 = this.createFromArgsTest("one", "1", "two", "2", "three",
                "3", "four", "4", "five", "5", "six", "6", "seven", "7", "eight", "8",
                "nine", "9", "ten", "10");
        int m1Expected = 10;
        /*
         * Assert that values of variables match expectations
         */
        assertEquals(m1Expected, m1.size());
    }
}

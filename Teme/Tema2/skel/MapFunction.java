import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.Callable;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * The type Map function.
 * Implements Callable instead of Runnable so we can return and process
 * the results of the tasks
 */
public class MapFunction implements Callable<MapResult> {

    private int id;
    private String document;
    private int offset;
    private int dimension;
    private Map<Integer, Integer> dictionary = new HashMap<>();
    private List<String> maxLengthWords = new ArrayList<>();
    private int maxLen = 0;

    /**
     * Instantiates a new Map function.
     *
     * @param id        the id
     * @param document  the document
     * @param offset    the offset
     * @param dimension the dimension
     */
    public MapFunction(int id, String document, int offset, int dimension) {
        this.setId(id);
        this.setDocument(document);
        this.setOffset(offset);
        this.setDimension(dimension);
    }

    @Override
    public MapResult call() throws Exception {

        BufferedReader reader = new BufferedReader(new FileReader(getDocument()));
        String text = readFromFile(reader, getOffset(), getDimension());

        // Add words from text to words array
        List<String> words = new ArrayList<>();
        words = addWordsToArray(words, text, false);

        // If first character of text is letter or digit -> the first letter(s) may represent the end of another word
        if (Character.isLetter(text.charAt(0)) || Character.isDigit(text.charAt(0))) {
            // If not first fragment, read previous fragment
            if (getOffset() != 0) {
                BufferedReader reader2 = new BufferedReader(new FileReader(getDocument()));
                String text2 = readFromFile(reader2, getOffset() - getDimension(), getDimension());

                // Check if last character from previous fragment is also letter or digit
                if (Character.isLetter(text2.charAt(getDimension() - 1)) ||
                        Character.isDigit(text2.charAt(getDimension() - 1))) {
                    // We drop the text because it represents the end of the word processed by the previous task
                    words.remove(0);
                }
            }
        }

        // If last character of text is letter or digit -> the last letter(s) may not form a complete word
        if (Character.isLetter(text.charAt(text.length() - 1)) ||
                Character.isDigit(text.charAt(text.length() - 1))) {
            // If not last fragment, read next fragment
            File textFile = new File(getDocument());
            long totalSize = textFile.length();
            if (getOffset() <= totalSize - getDimension()) {
                BufferedReader reader3 = new BufferedReader(new FileReader(getDocument()));
                String text3 = readFromFile(reader3, getOffset() + getDimension(), getDimension());

                // Check if first character from next fragment is also letter or digit
                if (Character.isLetter(text3.charAt(0)) || Character.isDigit(text3.charAt(0))) {
                    // We complete the text by adding the rest from the next task

                    // Find end of word from next task -> store it in words3
                    List<String> words3 = new ArrayList<>();
                    words3 = addWordsToArray(words3, text3, true);

                    // Get beginning of word from words
                    String beginning = words.get(words.size() - 1);
                    words.remove(words.size() - 1);

                    // Concatenate them
                    words.add(beginning + words3.get(0));

                    // Clear helper words array
                    words3.clear();
                }
            }
        }
        computeOutput(getId(), words);

        return new MapResult(getDocument(), getDictionary(), getMaxLengthWords());
    }

    /**
     * Compute output.
     * It interprets the data
     * Computes dictionary hashmap and maxLengthWords list -> the parameters for MapResult
     *
     * @param id   the id
     * @param list the list
     */
    public void computeOutput(int id, List<String> list) {

        for (String w : list) {
            // Add new entry in dictionary or increment key's value if
            // a word with the same length is found
            int count = getDictionary().getOrDefault(w.length(), 0);
            getDictionary().put(w.length(), count + 1);

            // If a longer word is found, update list and current maximum length
            if (w.length() > getMaxLen()) {
                getMaxLengthWords().clear();
                getMaxLengthWords().add(w);
                setMaxLen(w.length());

            // If a word with the maximum length is found, add it to the list
            } else if (w.length() == getMaxLen()) {
                getMaxLengthWords().add(w);
            }
        }
    }

    /**
     * Add words to array list.
     * Function that interprets and text and return of the words from that text
     * Words are identified by the following regex expression: [a-zA-Z0-9]+
     * ("+" because null words are not considered words)
     * Words <=> Separated by the following regex expression: ";:/?~\\.,><`[]{}()!@#$%^&-_+'=*\"| \t\r\n"
     *
     * @param words     the words
     * @param text      the text
     * @param onlyFirst the only first
     * @return the list
     */
    public List<String> addWordsToArray(List<String> words, String text, boolean onlyFirst) {

        Pattern p = Pattern.compile("[a-zA-Z0-9]+");
        Matcher m = p.matcher(text);
        // If only the first word from text is needed. Added for efficiency
        if (onlyFirst) {
            m.find();
            words.add(m.group());
        } else {
            while (m.find()) {
                words.add(m.group());
            }
        }

        return words;
    }

    /**
     * Read from file string.
     * Function that reads from a file starting from specific offset dimension number of bytes
     * Return String value of read fragment from text
     *
     * @param reader    the reader
     * @param offset    the offset
     * @param dimension the dimension
     * @return the string
     * @throws IOException the io exception
     */
    public String readFromFile(BufferedReader reader, int offset, int dimension)
            throws IOException {

        reader.skip(offset);

        StringBuilder text = new StringBuilder();
        char[] cbuf = new char[dimension];
        reader.read(cbuf, 0, dimension);
        for (int i = 0; i < dimension; i++) {
            if (cbuf[i] != (char) 0) {
                text.append(cbuf[i]);
            }
        }
        return String.valueOf(text);
    }

    /**
     * Gets id.
     *
     * @return the id
     */
    public int getId() {
        return id;
    }

    /**
     * Sets id.
     *
     * @param id the id
     */
    public void setId(int id) {
        this.id = id;
    }

    /**
     * Gets document.
     *
     * @return the document
     */
    public String getDocument() {
        return document;
    }

    /**
     * Sets document.
     *
     * @param document the document
     */
    public void setDocument(String document) {
        this.document = document;
    }

    /**
     * Gets offset.
     *
     * @return the offset
     */
    public int getOffset() {
        return offset;
    }

    /**
     * Sets offset.
     *
     * @param offset the offset
     */
    public void setOffset(int offset) {
        this.offset = offset;
    }

    /**
     * Gets dimension.
     *
     * @return the dimension
     */
    public int getDimension() {
        return dimension;
    }

    /**
     * Sets dimension.
     *
     * @param dimension the dimension
     */
    public void setDimension(int dimension) {
        this.dimension = dimension;
    }

    /**
     * Gets dictionary.
     *
     * @return the dictionary
     */
    public Map<Integer, Integer> getDictionary() {
        return dictionary;
    }

    /**
     * Sets dictionary.
     *
     * @param dictionary the dictionary
     */
    public void setDictionary(Map<Integer, Integer> dictionary) {
        this.dictionary = dictionary;
    }

    /**
     * Gets max length words.
     *
     * @return the max length words
     */
    public List<String> getMaxLengthWords() {
        return maxLengthWords;
    }

    /**
     * Sets max length words.
     *
     * @param maxLengthWords the max length words
     */
    public void setMaxLengthWords(List<String> maxLengthWords) {
        this.maxLengthWords = maxLengthWords;
    }

    /**
     * Gets max len.
     *
     * @return the max len
     */
    public int getMaxLen() {
        return maxLen;
    }

    /**
     * Sets max len.
     *
     * @param maxLen the max len
     */
    public void setMaxLen(int maxLen) {
        this.maxLen = maxLen;
    }
}

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.Scanner;
import java.util.Vector;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.stream.Collectors;

/**
 * Parser class. Implements the entire logic.
 * Coordinates map and reduce tasks.
 * Computes and formats output, writes it to out file
 */
public class Parser {

    private static int fragmentSize;
    private int numberOfFiles;
    private Vector<String> files = new Vector<>(getFragmentSize());
    private List<MapFunction> taskList = new ArrayList<>();
    private List<ReduceFunction> taskList2 = new ArrayList<>();
    private int numberOfWorkers;
    private ExecutorService executor;
    private String outputFile;

    /**
     * Instantiates a new Parser.
     *
     * @param input           the input
     * @param numberOfWorkers the number of workers
     * @param output          the output
     */
    public Parser(String input, int numberOfWorkers, String output) {

        this.setNumberOfWorkers(numberOfWorkers);
        setOutputFile(output);

        File file = new File(input);
        Scanner reader = null;
        try {
            reader = new Scanner(file);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        assert reader != null;
        setFragmentSize(Integer.parseInt(reader.nextLine()));
        setNumberOfFiles(Integer.parseInt(reader.nextLine()));

        for (int i = 0; i < getNumberOfFiles(); i++) {
            getFiles().add(reader.nextLine());
        }
        reader.close();
    }

    /**
     * Gets fragment size.
     *
     * @return the fragment size
     */
    public static int getFragmentSize() {
        return fragmentSize;
    }

    /**
     * Sets fragment size.
     *
     * @param fragmentSize the fragment size
     */
    public static void setFragmentSize(int fragmentSize) {
        Parser.fragmentSize = fragmentSize;
    }

    /**
     * Make tasks.
     * Creates both map and reduce tasks.
     * The executor will make the workers execute the tasks
     * Computes output
     */
    void makeTasks() {

        // Map
        List<MapResult> results = map();

        // Reduce
        List<ReduceResult> results2 = reduce(results);

        // Shutdown executor
        getExecutor().shutdown();

        // Compute output
        try {
            computeOutput(results2);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Map function
     * Creates MapFunction type tasks
     * Adds them to ExecutorService pool of tasks
     * Interprets results and returns a list of results (type: MapResult)
     *
     * @return the list
     */
    List<MapResult> map() {
        int id = 0;
        setExecutor(Executors.newFixedThreadPool(getNumberOfWorkers()));
        String file;

        // Map
        // Create map tasks for each file. Important parameters for tasks: document name,
        // offset to read from and the size of the fragment to be read
        for (int i = 0; i < getNumberOfFiles(); i++) {

            // Read file content
            file = "../" + getFiles().get(i);
            File textFile = new File(file);

            // Make tasks
            // Total file size -> needed to compute offsets for each task
            long totalSize = textFile.length();
            int currentTaskID = 0;
            while (totalSize >= 0) {
                int offset;
                if ((long) currentTaskID * getFragmentSize() > textFile.length()) {
                    offset = (int) totalSize;
                } else {
                    offset = currentTaskID * getFragmentSize();
                }
                MapFunction task = new MapFunction(id, file, offset, getFragmentSize());
                getTaskList().add(task);
                id++;
                currentTaskID++;
                totalSize -= getFragmentSize();
            }

        }

        // The workers will execute the tasks
        // The results of the tasks will be collected in the results array
        List<MapResult> results;
        results = executeAndCollect(getTaskList());

        return results;
    }

    /**
     * Reduce function
     * Creates ReduceFunction type tasks
     * Adds them to ExecutorService pool of tasks
     * Interprets results and returns a list of results (type: ReduceResult)
     *
     * @param results the results
     * @return the list
     */
    List<ReduceResult> reduce(List<MapResult> results) {

        // Create reduce tasks. The task parameter will be a MapResult object
        // in order to extract needed information for reduce process.
        // Each task will get the MapResults from the same file, so now one task
        // will process all the information from one file
        for (int i = 0; i < getNumberOfFiles(); i++) {
            List<MapResult> mapResults = new ArrayList<>();
            for (MapResult mapResult : results) {
                if (mapResult.getDocument().equals("../" + getFiles().get(i))) {
                    mapResults.add(mapResult);
                }
            }
            ReduceFunction task = new ReduceFunction(getFiles().get(i), mapResults);
            getTaskList2().add(task);
        }

        // The workers will execute the tasks
        // The results of the tasks will be collected in the results array
        List<ReduceResult> results2;
        results2 = executeAndCollect(getTaskList2());

        return results2;
    }

    /**
     * Execute and collect list.
     * Generic function
     *
     * @param <T>   the type parameter
     * @param tasks the tasks
     * @return the list
     */
    <T> List<T> executeAndCollect(Collection<? extends Callable<T>> tasks) {
        // invokeAll method executes the list of all Callable tasks in the "bag"
        List<Future<T>> resultList2 = null;
        try {
            resultList2 = getExecutor().invokeAll(tasks);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        // Extract list of results from type Future
        List<T> results2;
        assert resultList2 != null;
        results2 = resultList2.stream().map(resultFuture -> {
            try {
                return resultFuture.get();
            } catch (InterruptedException | ExecutionException e) {
                e.printStackTrace();
            }
            return null;
        }).collect(Collectors.toList());

        return results2;
    }

    /**
     * Compute output.
     * Sorts output decreasingly by rang
     * Writes output to outputfile
     *
     * @param results the results
     * @throws IOException the io exception
     */
    void computeOutput(List<ReduceResult> results) throws IOException {
        // Multiply with 100 because cast to int loses the 2 decimals of float list parameters
        // Sort result by rang
        results.sort((o1, o2) -> (int) (o2.getRang() * 100 - o1.getRang() * 100));

        FileWriter fileWriter = new FileWriter(getOutputFile());
        BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);

        for (ReduceResult reduceResult : results) {
            bufferedWriter.write(reduceResult.formatOutput());
        }
        bufferedWriter.close();
    }

    /**
     * Gets output file.
     *
     * @return the output file
     */
    public String getOutputFile() {
        return outputFile;
    }

    /**
     * Sets output file.
     *
     * @param outputFile the output file
     */
    public void setOutputFile(String outputFile) {
        this.outputFile = outputFile;
    }

    /**
     * Gets files.
     *
     * @return the files
     */
    public Vector<String> getFiles() {
        return files;
    }

    /**
     * Sets files.
     *
     * @param files the files
     */
    public void setFiles(Vector<String> files) {
        this.files = files;
    }

    /**
     * Gets number of files.
     *
     * @return the number of files
     */
    public int getNumberOfFiles() {
        return numberOfFiles;
    }

    /**
     * Sets number of files.
     *
     * @param numberOfFiles the number of files
     */
    public void setNumberOfFiles(int numberOfFiles) {
        this.numberOfFiles = numberOfFiles;
    }

    /**
     * Gets task list.
     *
     * @return the task list
     */
    public List<MapFunction> getTaskList() {
        return taskList;
    }

    /**
     * Sets task list.
     *
     * @param taskList the task list
     */
    public void setTaskList(List<MapFunction> taskList) {
        this.taskList = taskList;
    }

    /**
     * Gets task list 2.
     *
     * @return the task list 2
     */
    public List<ReduceFunction> getTaskList2() {
        return taskList2;
    }

    /**
     * Sets task list 2.
     *
     * @param taskList2 the task list 2
     */
    public void setTaskList2(List<ReduceFunction> taskList2) {
        this.taskList2 = taskList2;
    }

    /**
     * Gets number of workers.
     *
     * @return the number of workers
     */
    public int getNumberOfWorkers() {
        return numberOfWorkers;
    }

    /**
     * Sets number of workers.
     *
     * @param numberOfWorkers the number of workers
     */
    public void setNumberOfWorkers(int numberOfWorkers) {
        this.numberOfWorkers = numberOfWorkers;
    }

    /**
     * Gets executor.
     *
     * @return the executor
     */
    public ExecutorService getExecutor() {
        return executor;
    }

    /**
     * Sets executor.
     *
     * @param executor the executor
     */
    public void setExecutor(ExecutorService executor) {
        this.executor = executor;
    }
}

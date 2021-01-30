package webserver;

import core.*;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URLDecoder;
import java.nio.charset.StandardCharsets;
import java.time.Duration;
import java.time.LocalDateTime;
import java.util.Map;
import java.util.StringTokenizer;
import java.util.Vector;

import org.json.JSONObject;

// Based on
// https://www.ssaurel.com/blog/create-a-simple-http-web-server-in-java
// http://www.jcgonzalez.com/java-socket-mini-server-http-example

public class WebServer {
    private static final int PORT = 8080; // port to listen to

    private Element currentElement;
    private final Element root;
    private final Element active_root;
    private final Element totalTime_root;
    private final Element searchByTag_root;

    public WebServer(Element root, Element active_root, Element totalTime_root, Element searchByTag_root) {
        this.root = root;
        this.active_root = active_root;
        this.totalTime_root = totalTime_root;
        this.searchByTag_root = searchByTag_root;

        System.out.println(root);
        currentElement = root;
        try {
            ServerSocket serverConnect = new ServerSocket(PORT);
            System.out.println("Server started.\nListening for connections on port : " + PORT + " ...\n");
            // we listen until user halts server execution
            while (true) {
                // each client connection will be managed in a dedicated Thread
                new SocketThread(serverConnect.accept());
                // create dedicated thread to manage the client connection
            }
        } catch (IOException e) {
            System.err.println("Server Connection error : " + e.getMessage());
        }
    }

    private Element findActivityById(int id) {
        return root.findActivityById(id);
    }

    private void setActiveTasksRoot(Vector<Element> active_tasks) { active_root.setM_element_array(active_tasks); }

    public void setTotalTimeRoot(Vector<Element> totalTimeResults) { totalTime_root.setM_element_array(totalTimeResults); }

    public void setSearchByTagRoot(Vector<Element> searchByTagResults) { searchByTag_root.setM_element_array(searchByTagResults); }

    private class SocketThread extends Thread {
        // SocketThread sees WebServer attributes
        private final Socket insocked;
        // Client Connection via Socket Class

        SocketThread(Socket insocket) {
            this.insocked = insocket;
            this.start();
        }

        @Override
        public void run() {
            // we manage our particular client connection
            BufferedReader in;
            PrintWriter out;
            String resource;

            try {
                // we read characters from the client via input stream on the socket
                in = new BufferedReader(new InputStreamReader(insocked.getInputStream()));
                // we get character output stream to client
                out = new PrintWriter(insocked.getOutputStream());
                // get first line of the request from the client
                String input = in.readLine();
                // we parse the request with a string tokenizer

                System.out.println("sockedthread : " + input);

                StringTokenizer parse = new StringTokenizer(input);
                String method = parse.nextToken().toUpperCase();
                // we get the HTTP method of the client
                if (!method.equals("GET")) {
                    System.out.println("501 Not Implemented : " + method + " method.");
                } else {
                    // what comes after "localhost:8080"
                    resource = parse.nextToken();
                    System.out.println("input " + input);
                    System.out.println("method " + method);
                    System.out.println("resource " + resource);

                    parse = new StringTokenizer(resource, "/[?]=&");
                    int i = 0;
                    String[] tokens = new String[20];
                    // more than the actual number of parameters
                    while (parse.hasMoreTokens()) {
                        tokens[i] = parse.nextToken();
                        System.out.println("token " + i + "=" + tokens[i]);
                        i++;
                    }

                    // Make the answer as a JSON string, to be sent to the Javascript client
                    String answer = makeHeaderAnswer() + makeBodyAnswer(tokens);
                    System.out.println("answer\n" + answer);
                    // Here we send the response to the client
                    out.println(answer);
                    out.flush(); // flush character output stream buffer
                }

                in.close();
                out.close();
                insocked.close(); // we close socket connection
            } catch (Exception e) {
                System.err.println("Exception : " + e);
            }
        }


        private String makeBodyAnswer(String[] tokens) {
            String body = "";
            switch (tokens[0]) {
                case "get_tree" : {
                    int id = Integer.parseInt(tokens[1]);
                    Element element = findActivityById(id);
                    assert (element!=null);
                    body = element.toJson(1).toString();
                    break;
                }

                case "reorder" : {
                    int id = Integer.parseInt(tokens[1]);
                    Element element = findActivityById(id);

                    int oldIndex = Integer.parseInt(tokens[2]);
                    int newIndex = Integer.parseInt(tokens[3]);
                    Element item = element.getM_element_array().get(oldIndex);

                    element.getM_element_array().remove(oldIndex);
                    element.getM_element_array().insertElementAt(item, newIndex);

                    assert (item != null);
                    body = "{}";
                    break;
                }

                case "start": {
                    int id = Integer.parseInt(tokens[1]);
                    Element element = findActivityById(id);
                    assert (element!=null);
                    Task task = (Task) element;
                    task.createInterval();
                    body = "{}";
                    break;
                }
                case "stop": {
                    int id = Integer.parseInt(tokens[1]);
                    Element element = findActivityById(id);
                    assert (element!=null);
                    Task task = (Task) element;
                    task.stopInterval();
                    body = "{}";
                    break;
                }

                case "running":{
                    int id = Integer.parseInt(tokens[1]);
                    setActiveTasksRoot(findActivityById(id).findActiveTasks());
                    // assert (element != null);
                    body = active_root.toJson(1).toString();
                    break;
                }

                case "add":{
                    int id = Integer.parseInt(tokens[1]);
                    String name = tokens[2];
                    name = URLDecoder.decode(name, StandardCharsets.UTF_8); // Desencriptación UTF-8

                    String type = tokens[3];

                    Project element = (Project) findActivityById(id); // El padre siempre será un proyecto
                    Element newElement = null;

                    if (type.equals("Project"))
                        newElement = element.createProject(name);
                    else if (type.equals("Task"))
                        newElement = element.createTask(name);

                    // We want added elements to appear on top, not on the bottom:
                    element.getM_element_array().insertElementAt(element.getM_element_array().lastElement(), 0);
                    element.getM_element_array().remove(element.getM_element_array().size()-1);

                    if (4 < tokens.length) { // Si hay tags en los tokens...
                        int i = 4;
                        while(i < tokens.length) {
                            if (tokens[i] != null) {
                                assert newElement != null;
                                newElement.addTag(URLDecoder.decode(tokens[i], StandardCharsets.UTF_8));
                                i = i + 1;
                            }
                            else
                                break;
                        }
                    }
                    // assert (element != null);
                    body = "{}";
                    break;
                }

                case "remove":{
                    int id = Integer.parseInt(tokens[1]);
                    int index = Integer.parseInt(tokens[2]);

                    Element element = findActivityById(id);
                    element.removeElement(element.getM_element_array().get(index));


                    body = "{}";
                    break;
                }

                case "searchByTime":{
                    int id = 0;

                    LocalDateTime initDateTime = LocalDateTime.of(Integer.parseInt(tokens[2]), Integer.parseInt(tokens[3]), Integer.parseInt(tokens[4]), Integer.parseInt(tokens[5]), Integer.parseInt(tokens[6]));
                    LocalDateTime endDateTime = LocalDateTime.of(Integer.parseInt(tokens[7]), Integer.parseInt(tokens[8]), Integer.parseInt(tokens[9]), Integer.parseInt(tokens[10]), Integer.parseInt(tokens[11]));
                    Element element = findActivityById(id);

                    TotalTime it = element.createTotalTimeIterator(initDateTime, endDateTime);

                    Map<Element, Duration> keyPair = Utils.TotalTime(it);

                    Vector<Element> keySet_vector = new Vector<>(keyPair.keySet());

                    Vector<Duration> duration_vector = new Vector<>(keyPair.values());

                    Vector<Element> final_vector = new Vector<>();

                    Vector<Duration> final_duration_vector = new Vector<>();

                    int index = 0;
                    if (keySet_vector.size() > 0)
                        while (index < keySet_vector.size()) {
                            if (!(keySet_vector.get(index).getDuration().equals(Duration.ZERO))) {
                                final_vector.add(keySet_vector.get(index));
                                final_duration_vector.add(duration_vector.get(index));
                            }
                            index = index+1;
                        }


                    setTotalTimeRoot(final_vector);

                    body = totalTime_root.toJson(1).toString();
                    break;

                }

                case "searchByTag":{

                    String tag = tokens[2];

                    SearchByTag it = root.createTagIterator(tag);

                    Vector<Element> searchByTagResult = Utils.getTagResult(it);

                    setSearchByTagRoot(searchByTagResult);

                    body = searchByTag_root.toJson(1).toString();

                    break;

                }


                case "totalTime":{

                    int id = 0;

                    LocalDateTime initDateTime = LocalDateTime.of(Integer.parseInt(tokens[2]), Integer.parseInt(tokens[3]), Integer.parseInt(tokens[4]), Integer.parseInt(tokens[5]), Integer.parseInt(tokens[6]));
                    LocalDateTime endDateTime = LocalDateTime.of(Integer.parseInt(tokens[7]), Integer.parseInt(tokens[8]), Integer.parseInt(tokens[9]), Integer.parseInt(tokens[10]), Integer.parseInt(tokens[11]));

                    TotalTime it = root.createTotalTimeIterator(initDateTime, endDateTime);

                    int totalTimeResult = (int) Utils.TotalTime_calculation(it).toSeconds();
                    JSONObject totalTimeResultJSON = new JSONObject();
                    totalTimeResultJSON.put("value", totalTimeResult);

                    body = totalTimeResultJSON.toString();

                    break;

                }
                // TODO: add new task, project
                // TODO: edit task, project properties
                default:
                    assert false;
            }
            System.out.println(body);
            return body;
        }

        private String makeHeaderAnswer() {
            String answer = "";
            answer += "HTTP/1.0 200 OK\r\n";
            answer += "Content-type: application/json\r\n";
            answer += "\r\n";
            // blank line between headers and content, very important !
            return answer;
        }
    } // SocketThread


} // WebServer


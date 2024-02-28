package udp.java_impl;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.UnknownHostException;

public class Server {
    private static final String SERVER_ADDRESS = "localhost";
    private static final short SERVER_PORT = 1234;

    public static void main(String[] args) throws UnknownHostException{
        try(DatagramSocket server = new DatagramSocket(SERVER_PORT, InetAddress.getByName(SERVER_ADDRESS))) {
            byte[] recvData = new byte[1024];

            // recvPacket where we will received data
            DatagramPacket recvPacket = new DatagramPacket(recvData, recvData.length);
            server.receive(recvPacket);

            // string built on the bytes from the recvPacket
            String recvStr = new String(recvPacket.getData());
            System.out.println(recvStr);

            // receiving the ip address and port of the client to send back data
            String string = "world";
            InetAddress ipAddress = recvPacket.getAddress();
            int port = recvPacket.getPort();

            DatagramPacket sendPacket = new DatagramPacket(string.getBytes(), string.length(), ipAddress, port);
            server.send(sendPacket);

        } catch (IOException iOE) {
            System.err.println(iOE.getMessage());
        }
    }   
}

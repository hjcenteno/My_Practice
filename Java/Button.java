package Java;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;

public class Button implements ActionListener {
    public JPanel panel;
    public JFrame frame;
    public JLabel label;
    public JButton button;
    public int count=0;
    Button(){
        frame = new JFrame();

        button = new JButton("Click me");
        button.addActionListener(this);

        label=new JLabel("Number of clicks: 0");

        panel=new JPanel();
        panel.setBorder(BorderFactory.createEmptyBorder(100, 100, 100, 100));
        panel.setLayout(new GridLayout(0, 1));
        panel.add(button);
        panel.add(label);

        frame.add(panel, BorderLayout.CENTER);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setTitle("Button");
        frame.pack();
        frame.setVisible(true);
    }

    public static void main(String[] args) {
        new Button();
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        // TODO Auto-generated method stub
       count++;
       label.setText("numebr of clicks: "+count);
    }
}

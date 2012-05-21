/*
 * AWG2021Setup.java
 *
 * Created on July 24, 2008, 5:04 PM
 */



/**
 *
 * @author  Administrator
 */
public class AWG2021Setup extends DeviceSetup {

    /** Creates new form AWG2021Setup */
    public AWG2021Setup() {
        initComponents();
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        deviceButtons1 = new DeviceButtons();
        jScrollPane1 = new javax.swing.JScrollPane();
        jPanel1 = new javax.swing.JPanel();
        jPanel2 = new javax.swing.JPanel();
        jPanel5 = new javax.swing.JPanel();
        deviceField8 = new DeviceField();
        deviceField9 = new DeviceField();
        deviceDispatch1 = new DeviceDispatch();
        jPanel7 = new javax.swing.JPanel();
        deviceChoice3 = new DeviceChoice();
        deviceField3 = new DeviceField();
        deviceChoice4 = new DeviceChoice();
        deviceField29 = new DeviceField();
        jPanel4 = new javax.swing.JPanel();
        jPanel10 = new javax.swing.JPanel();
        deviceField12 = new DeviceField();
        deviceField13 = new DeviceField();
        deviceField14 = new DeviceField();
        deviceField16 = new DeviceField();
        deviceField15 = new DeviceField();
        deviceChoice5 = new DeviceChoice();

        setDeviceProvider("localhost");
        setDeviceTitle("AWG2021");
        setDeviceType("AWG2021");
        setHeight(250);
        setWidth(900);

        deviceButtons1.setCheckExpressions(new String[] {});
        deviceButtons1.setCheckMessages(new String[] {});
        deviceButtons1.setMethods(new String[] {"INIT", "ARM", "TRIGGER", "RESET"});
        getContentPane().add(deviceButtons1, java.awt.BorderLayout.SOUTH);

        jPanel1.setLayout(new java.awt.BorderLayout());

        jPanel2.setLayout(new java.awt.GridLayout(2, 0));

        jPanel5.setBorder(javax.swing.BorderFactory.createTitledBorder("GENERAL"));
        jPanel5.setLayout(new java.awt.FlowLayout(java.awt.FlowLayout.LEFT, 0, 0));

        deviceField8.setIdentifier("");
        deviceField8.setLabelString("GPIB ADDR:");
        deviceField8.setNumCols(5);
        deviceField8.setOffsetNid(1);
        jPanel5.add(deviceField8);

        deviceField9.setIdentifier("");
        deviceField9.setLabelString("COMMENT:");
        deviceField9.setNumCols(40);
        deviceField9.setOffsetNid(2);
        jPanel5.add(deviceField9);
        jPanel5.add(deviceDispatch1);

        jPanel2.add(jPanel5);

        jPanel7.setBorder(javax.swing.BorderFactory.createTitledBorder("TRIGGER"));
        jPanel7.setLayout(new java.awt.FlowLayout(java.awt.FlowLayout.LEFT, 0, 0));

        deviceChoice3.setChoiceItems(new String[] {"POSITIVE", "NEGATIVE"});
        deviceChoice3.setIdentifier("");
        deviceChoice3.setLabelString("SLOPE:");
        deviceChoice3.setOffsetNid(5);
        deviceChoice3.setUpdateIdentifier("");
        jPanel7.add(deviceChoice3);

        deviceField3.setIdentifier("");
        deviceField3.setLabelString("LEVEL (V):");
        deviceField3.setNumCols(5);
        deviceField3.setOffsetNid(4);
        jPanel7.add(deviceField3);

        deviceChoice4.setChoiceItems(new String[] {"HIGH", "LOW"});
        deviceChoice4.setIdentifier("");
        deviceChoice4.setLabelString("IMPEDANCE:");
        deviceChoice4.setOffsetNid(6);
        deviceChoice4.setUpdateIdentifier("");
        jPanel7.add(deviceChoice4);

        deviceField29.setIdentifier("");
        deviceField29.setLabelString("SOURCE:");
        deviceField29.setNumCols(25);
        deviceField29.setOffsetNid(7);
        jPanel7.add(deviceField29);

        jPanel2.add(jPanel7);

        jPanel1.add(jPanel2, java.awt.BorderLayout.NORTH);

        jPanel4.setBorder(javax.swing.BorderFactory.createTitledBorder("WFM"));
        jPanel4.setLayout(new java.awt.GridLayout(1, 0));

        jPanel10.setLayout(new java.awt.FlowLayout(java.awt.FlowLayout.LEFT, 0, 0));

        deviceField12.setIdentifier("");
        deviceField12.setLabelString("AMPLITUDE (Vpp):");
        deviceField12.setNumCols(5);
        deviceField12.setOffsetNid(9);
        jPanel10.add(deviceField12);

        deviceField13.setIdentifier("");
        deviceField13.setLabelString("OFFSET (V):");
        deviceField13.setNumCols(5);
        deviceField13.setOffsetNid(10);
        jPanel10.add(deviceField13);

        deviceField14.setIdentifier("");
        deviceField14.setLabelString("FREQUENCY (Hz):");
        deviceField14.setNumCols(6);
        deviceField14.setOffsetNid(13);
        jPanel10.add(deviceField14);

        deviceField16.setIdentifier("");
        deviceField16.setLabelString("POINTS:");
        deviceField16.setNumCols(5);
        deviceField16.setOffsetNid(14);
        jPanel10.add(deviceField16);

        deviceField15.setIdentifier("");
        deviceField15.setLabelString("DURATION (s):");
        deviceField15.setNumCols(4);
        deviceField15.setOffsetNid(12);
        jPanel10.add(deviceField15);

        deviceChoice5.setChoiceItems(new String[] {"OFF", "1MHz", "5MHz", "20MHz", "50MHz"});
        deviceChoice5.setIdentifier("");
        deviceChoice5.setLabelString("FILTER:");
        deviceChoice5.setOffsetNid(11);
        deviceChoice5.setUpdateIdentifier("");
        jPanel10.add(deviceChoice5);

        jPanel4.add(jPanel10);

        jPanel1.add(jPanel4, java.awt.BorderLayout.CENTER);

        jScrollPane1.setViewportView(jPanel1);

        getContentPane().add(jScrollPane1, java.awt.BorderLayout.CENTER);
    }// </editor-fold>//GEN-END:initComponents


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private DeviceButtons deviceButtons1;
    private DeviceChoice deviceChoice3;
    private DeviceChoice deviceChoice4;
    private DeviceChoice deviceChoice5;
    private DeviceDispatch deviceDispatch1;
    private DeviceField deviceField12;
    private DeviceField deviceField13;
    private DeviceField deviceField14;
    private DeviceField deviceField15;
    private DeviceField deviceField16;
    private DeviceField deviceField29;
    private DeviceField deviceField3;
    private DeviceField deviceField8;
    private DeviceField deviceField9;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel10;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel4;
    private javax.swing.JPanel jPanel5;
    private javax.swing.JPanel jPanel7;
    private javax.swing.JScrollPane jScrollPane1;
    // End of variables declaration//GEN-END:variables

}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * MARTE_MHD_BTSetup.java
 *
 * Created on Mar 2, 2011, 1:07:35 PM
 */

/**
 *
 * @author manduchi
 */
public class MARTE_MHD_BTSetup extends DeviceSetup {

    /** Creates new form MARTE_MHD_BTSetup */
    public MARTE_MHD_BTSetup() {
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
        jPanel2 = new javax.swing.JPanel();
        jPanel5 = new javax.swing.JPanel();
        deviceField1 = new DeviceField();
        jPanel6 = new javax.swing.JPanel();
        deviceField2 = new DeviceField();
        deviceField3 = new DeviceField();
        jPanel7 = new javax.swing.JPanel();
        deviceField4 = new DeviceField();
        deviceField5 = new DeviceField();
        jPanel8 = new javax.swing.JPanel();
        deviceChoice1 = new DeviceChoice();
        deviceField6 = new DeviceField();
        jPanel9 = new javax.swing.JPanel();
        deviceField7 = new DeviceField();
        deviceField8 = new DeviceField();
        jPanel1 = new javax.swing.JPanel();
        jTabbedPane1 = new javax.swing.JTabbedPane();
        jPanel3 = new javax.swing.JPanel();
        deviceTable3 = new DeviceTable();
        deviceTable4 = new DeviceTable();
        jPanel4 = new javax.swing.JPanel();
        deviceChoice2 = new DeviceChoice();
        deviceField9 = new DeviceField();
        deviceField10 = new DeviceField();
        deviceField11 = new DeviceField();
        deviceField12 = new DeviceField();
        deviceField13 = new DeviceField();
        deviceField14 = new DeviceField();
        deviceField15 = new DeviceField();
        deviceField16 = new DeviceField();
        deviceField17 = new DeviceField();
        deviceField18 = new DeviceField();
        deviceField19 = new DeviceField();
        deviceField20 = new DeviceField();
        jPanel10 = new javax.swing.JPanel();
        deviceTable1 = new DeviceTable();
        jPanel11 = new javax.swing.JPanel();
        deviceChoice3 = new DeviceChoice();
        deviceTable2 = new DeviceTable();

        setDeviceProvider("localhost");
        setDeviceTitle("MARTe MhdBt Setup");
        setDeviceType("MARTE_MHD_BT");
        setHeight(400);
        setWidth(600);
        getContentPane().add(deviceButtons1, java.awt.BorderLayout.PAGE_END);

        jPanel2.setLayout(new java.awt.GridLayout(5, 0));

        deviceField1.setIdentifier("");
        deviceField1.setLabelString("Comment: ");
        deviceField1.setNumCols(30);
        deviceField1.setOffsetNid(1);
        deviceField1.setTextOnly(true);
        jPanel5.add(deviceField1);

        jPanel2.add(jPanel5);

        deviceField2.setIdentifier("");
        deviceField2.setLabelString("Start Sampling(s):");
        deviceField2.setOffsetNid(8);
        jPanel6.add(deviceField2);

        deviceField3.setIdentifier("");
        deviceField3.setLabelString("End Sampling(s):");
        deviceField3.setOffsetNid(9);
        jPanel6.add(deviceField3);

        jPanel2.add(jPanel6);

        deviceField4.setIdentifier("");
        deviceField4.setLabelString("Start Offset Comp(s).:");
        deviceField4.setOffsetNid(6);
        jPanel7.add(deviceField4);

        deviceField5.setIdentifier("");
        deviceField5.setLabelString("End Offset Comp.(s):");
        deviceField5.setOffsetNid(7);
        jPanel7.add(deviceField5);

        jPanel2.add(jPanel7);

        deviceChoice1.setChoiceItems(new String[] {"MhdBrControl"});
        deviceChoice1.setIdentifier("");
        deviceChoice1.setLabelString("Control: ");
        deviceChoice1.setOffsetNid(12);
        deviceChoice1.setUpdateIdentifier("");
        jPanel8.add(deviceChoice1);

        deviceField6.setIdentifier("");
        deviceField6.setLabelString("Contr. Duration(s): ");
        deviceField6.setOffsetNid(5);
        jPanel8.add(deviceField6);

        jPanel2.add(jPanel8);

        deviceField7.setIdentifier("");
        deviceField7.setLabelString("Trig. Time: ");
        deviceField7.setNumCols(25);
        deviceField7.setOffsetNid(4);
        jPanel9.add(deviceField7);

        deviceField8.setIdentifier("");
        deviceField8.setLabelString("Freq. (Hz):");
        deviceField8.setOffsetNid(3);
        jPanel9.add(deviceField8);

        jPanel2.add(jPanel9);

        getContentPane().add(jPanel2, java.awt.BorderLayout.PAGE_START);

        jPanel1.setLayout(new java.awt.BorderLayout());

        jPanel3.setLayout(new java.awt.GridLayout(1, 2));

        deviceTable3.setDisplayRowNumber(true);
        deviceTable3.setIdentifier("");
        deviceTable3.setLabelString("Mapping:");
        deviceTable3.setNumCols(1);
        deviceTable3.setNumRows(192);
        deviceTable3.setOffsetNid(1339);
        deviceTable3.setPreferredColumnWidth(60);
        deviceTable3.setPreferredHeight(200);
        deviceTable3.setUseExpressions(true);
        jPanel3.add(deviceTable3);

        deviceTable4.setBinary(true);
        deviceTable4.setDisplayRowNumber(true);
        deviceTable4.setIdentifier("");
        deviceTable4.setLabelString("Autozero Mask");
        deviceTable4.setNumCols(1);
        deviceTable4.setNumRows(192);
        deviceTable4.setOffsetNid(1345);
        deviceTable4.setPreferredColumnWidth(4);
        deviceTable4.setPreferredHeight(200);
        jPanel3.add(deviceTable4);

        jTabbedPane1.addTab("Mapping&Offset", jPanel3);

        deviceChoice2.setChoiceIntValues(new int[] {0, 1, 2, 3, 4, 5, 6, 7, 8});
        deviceChoice2.setChoiceItems(new String[] {"NONE", "MODE", "MODE_AND_PHASE", "RATIO", "RATIO_AND_PHASE", "MODE_M0_N7", "MODE_AND_PHASE_M0_N7", "MODE_M0_N1", "MODE_AND_PHASE_M0_N1"});
        deviceChoice2.setConvert(true);
        deviceChoice2.setIdentifier("");
        deviceChoice2.setLabelString("Trig. Mode: ");
        deviceChoice2.setOffsetNid(1351);
        deviceChoice2.setUpdateIdentifier("");
        jPanel4.add(deviceChoice2);

        deviceField9.setIdentifier("");
        deviceField9.setLabelString("Min Amp.: ");
        deviceField9.setNumCols(6);
        deviceField9.setOffsetNid(1357);
        jPanel4.add(deviceField9);

        deviceField10.setIdentifier("");
        deviceField10.setLabelString("Max Amp.: ");
        deviceField10.setNumCols(6);
        deviceField10.setOffsetNid(1363);
        jPanel4.add(deviceField10);

        deviceField11.setIdentifier("");
        deviceField11.setLabelString("Min Phase.: ");
        deviceField11.setNumCols(6);
        deviceField11.setOffsetNid(1369);
        jPanel4.add(deviceField11);

        deviceField12.setIdentifier("");
        deviceField12.setLabelString("Max Phase: ");
        deviceField12.setNumCols(6);
        deviceField12.setOffsetNid(1375);
        jPanel4.add(deviceField12);

        deviceField13.setIdentifier("");
        deviceField13.setLabelString("Min Ratio:  ");
        deviceField13.setNumCols(6);
        deviceField13.setOffsetNid(1381);
        jPanel4.add(deviceField13);

        deviceField14.setIdentifier("");
        deviceField14.setLabelString("Max Ratio: ");
        deviceField14.setNumCols(6);
        deviceField14.setOffsetNid(1387);
        jPanel4.add(deviceField14);

        deviceField15.setIdentifier("");
        deviceField15.setLabelString("N. Times:");
        deviceField15.setNumCols(6);
        deviceField15.setOffsetNid(1393);
        jPanel4.add(deviceField15);

        deviceField16.setIdentifier("");
        deviceField16.setLabelString("Trig. Times:");
        deviceField16.setNumCols(6);
        deviceField16.setOffsetNid(1399);
        jPanel4.add(deviceField16);

        deviceField17.setIdentifier("");
        deviceField17.setLabelString("Duration: ");
        deviceField17.setNumCols(6);
        deviceField17.setOffsetNid(1405);
        jPanel4.add(deviceField17);

        deviceField18.setIdentifier("");
        deviceField18.setLabelString("Threshold: ");
        deviceField18.setNumCols(6);
        deviceField18.setOffsetNid(1411);
        jPanel4.add(deviceField18);

        deviceField19.setIdentifier("");
        deviceField19.setLabelString("Inhibit: ");
        deviceField19.setNumCols(6);
        deviceField19.setOffsetNid(1417);
        jPanel4.add(deviceField19);

        deviceField20.setIdentifier("");
        deviceField20.setLabelString("End Time: ");
        deviceField20.setNumCols(6);
        deviceField20.setOffsetNid(1423);
        jPanel4.add(deviceField20);

        jTabbedPane1.addTab("Thomson Triggers", jPanel4);

        jPanel10.setLayout(new java.awt.BorderLayout());

        deviceTable1.setColumnNames(new String[] {"Gain", "Offset"});
        deviceTable1.setDisplayRowNumber(true);
        deviceTable1.setIdentifier("");
        deviceTable1.setNumCols(2);
        deviceTable1.setNumRows(192);
        deviceTable1.setOffsetNid(10);
        deviceTable1.setUseExpressions(true);
        jPanel10.add(deviceTable1, java.awt.BorderLayout.CENTER);

        jTabbedPane1.addTab("In Calibration", jPanel10);

        deviceChoice3.setChoiceIntValues(new int[] {0, 1, 2});
        deviceChoice3.setChoiceItems(new String[] {"None", "Ip", "Bp"});
        deviceChoice3.setConvert(true);
        deviceChoice3.setIdentifier("");
        deviceChoice3.setLabelString("Mode: ");
        deviceChoice3.setOffsetNid(1429);
        deviceChoice3.setUpdateIdentifier("");
        jPanel11.add(deviceChoice3);

        deviceTable2.setIdentifier("");
        deviceTable2.setLabelString("Corr. Factors: ");
        deviceTable2.setNumCols(1);
        deviceTable2.setNumRows(192);
        deviceTable2.setOffsetNid(1435);
        jPanel11.add(deviceTable2);

        jTabbedPane1.addTab("Correction", jPanel11);

        jPanel1.add(jTabbedPane1, java.awt.BorderLayout.CENTER);

        getContentPane().add(jPanel1, java.awt.BorderLayout.CENTER);
    }// </editor-fold>//GEN-END:initComponents


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private DeviceButtons deviceButtons1;
    private DeviceChoice deviceChoice1;
    private DeviceChoice deviceChoice2;
    private DeviceChoice deviceChoice3;
    private DeviceField deviceField1;
    private DeviceField deviceField10;
    private DeviceField deviceField11;
    private DeviceField deviceField12;
    private DeviceField deviceField13;
    private DeviceField deviceField14;
    private DeviceField deviceField15;
    private DeviceField deviceField16;
    private DeviceField deviceField17;
    private DeviceField deviceField18;
    private DeviceField deviceField19;
    private DeviceField deviceField2;
    private DeviceField deviceField20;
    private DeviceField deviceField3;
    private DeviceField deviceField4;
    private DeviceField deviceField5;
    private DeviceField deviceField6;
    private DeviceField deviceField7;
    private DeviceField deviceField8;
    private DeviceField deviceField9;
    private DeviceTable deviceTable1;
    private DeviceTable deviceTable2;
    private DeviceTable deviceTable3;
    private DeviceTable deviceTable4;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel10;
    private javax.swing.JPanel jPanel11;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JPanel jPanel4;
    private javax.swing.JPanel jPanel5;
    private javax.swing.JPanel jPanel6;
    private javax.swing.JPanel jPanel7;
    private javax.swing.JPanel jPanel8;
    private javax.swing.JPanel jPanel9;
    private javax.swing.JTabbedPane jTabbedPane1;
    // End of variables declaration//GEN-END:variables

}

#include <iostream>

#include <thrift_endpoint.h>

#include <pd/pd_tables.h>
#include <pd/pd_static.h>

int main() {
  start_server();

  p4_pd_init();
  
  p4_pd_sess_hdl_t sess_hdl;
  p4_pd_client_init(&sess_hdl, 16);
  
  std::cerr << "session handle is " << sess_hdl << std::endl;
  
  p4_pd_dev_target_t dev_tgt = {0, 0xFF};
  p4_pd_entry_hdl_t entry_hdl;
  
  /* TEST BEGIN */
  
  p4_pd_test_actionA_action_spec actionA_action_spec =
    {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
  p4_pd_test_actionB_action_spec actionB_action_spec =
    {0xab};
  
  // right now PD assumes everything is passed in network byte order, so this
  // will actually be interpreted as byte string "bb00aa00"
  p4_pd_test_ExactOne_match_spec_t ExactOne_match_spec = {0x00aa00bb};
  p4_pd_test_ExactOne_table_add_with_actionA(sess_hdl, dev_tgt,
                                             &ExactOne_match_spec,
                                             &actionA_action_spec,
                                             &entry_hdl);

  p4_pd_test_ExactOne_table_modify_with_actionB(sess_hdl, dev_tgt.device_id,
                                                entry_hdl, &actionB_action_spec);
  
  p4_pd_test_ExactOne_table_delete(sess_hdl, dev_tgt.device_id, entry_hdl);
  
  p4_pd_test_LpmOne_match_spec_t LpmOne_match_spec = {0x12345678, 12};
  p4_pd_test_LpmOne_table_add_with_actionA(sess_hdl, dev_tgt,
                                           &LpmOne_match_spec,
                                           &actionA_action_spec,
                                           &entry_hdl);

  p4_pd_test_TernaryOne_match_spec_t TernaryOne_match_spec = {0x10101010,
                                                              0xff000a00};
  p4_pd_test_TernaryOne_table_add_with_actionA(sess_hdl, dev_tgt,
                                               &TernaryOne_match_spec,
                                               22 /* priority */,
                                               &actionA_action_spec,
                                               &entry_hdl);
  
  p4_pd_test_ExactOneNA_match_spec_t ExactOneNA_match_spec = {0xdebc0a};
  p4_pd_test_ExactOneNA_table_add_with_actionA(sess_hdl, dev_tgt,
                                               &ExactOneNA_match_spec,
                                               &actionA_action_spec,
                                               &entry_hdl);
  
  p4_pd_test_ExactTwo_match_spec_t ExactTwo_match_spec = {0xaabbccdd, 0xeeff};
  p4_pd_test_ExactTwo_table_add_with_actionA(sess_hdl, dev_tgt,
                                             &ExactTwo_match_spec,
                                             &actionA_action_spec,
                                             &entry_hdl);
  
  p4_pd_test_ExactOne_set_default_action_actionA(sess_hdl, dev_tgt,
                                                 &actionA_action_spec,
                                                 &entry_hdl);
  
  
  return 0;
}
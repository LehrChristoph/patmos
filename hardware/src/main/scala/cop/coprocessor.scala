
package cop

import Chisel._

import patmos._
import patmos.Constants._
import util._
import ocp._

abstract class CoprocessorObject() {
  // every device object must have methods "create" and "init"
  def init(config: Config#CoprocessorConfig)
  def create(config: Config#CoprocessorConfig) : Coprocessor
}

abstract class Coprocessor() extends Module() {
    val io = new  CoprocessorIO()
}

abstract class CoprocessorMemory() extends Coprocessor(){
    val memPort = new OcpBurstMasterPort(EXTMEM_ADDR_WIDTH, DATA_WIDTH, BURST_LENGTH)
}
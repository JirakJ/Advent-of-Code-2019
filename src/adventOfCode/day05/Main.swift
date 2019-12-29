import Foundation

typealias Program = [Int]

typealias Address = Int

enum ParameterMode: Int {
  case position = 0
  case immediate = 1

  func valueFrom(program: Program, address: Address) -> Int {
    switch self {
    case .position:
      return program[address]
    case .immediate:
      return address
    }
  }
}

struct ParameterModes {
  let first: ParameterMode
  let second: ParameterMode
  let third: ParameterMode  // Defined by the problem, but not used
  init(optcode: Int) {
    self.first = ParameterMode(rawValue: optcode / 100 % 10)!
    self.second = ParameterMode(rawValue: optcode / 1000 % 10)!
    self.third = ParameterMode(rawValue: optcode / 10000 % 10)!
  }
}

protocol Instruction {
  func execute(program: inout Program, address: Address, paramModes: ParameterModes) -> Address
}

struct WriteInstruction: Instruction {
  let operation: (Int, Int) -> Int

  func execute(program: inout Program, address: Address, paramModes: ParameterModes) -> Address {
    let leftAddress = program[address + 1]
    let rightAddress = program[address + 2]
    let resultAddress = program[address + 3]

    let left = paramModes.first.valueFrom(program: program, address: leftAddress)
    let right = paramModes.second.valueFrom(program: program, address: rightAddress)
    let result = operation(left, right)

    program[resultAddress] = result

    return address + 4
  }
}

struct InputInstruction: Instruction {
  let input: Int

  func execute(program: inout Program, address: Address, paramModes: ParameterModes) -> Address {
    let inputAddress = program[address + 1]

    program[inputAddress] = input

    return address + 2
  }
}

struct OutputInstruction: Instruction {
  let operation: (Int) -> Void

  func execute(program: inout Program, address: Address, paramModes: ParameterModes) -> Address {
    let outputAddress = program[address + 1]
    let output = paramModes.first.valueFrom(program: program, address: outputAddress)

    operation(output)

    return address + 2
  }
}

struct JumpInstruction: Instruction {
  let operation: (Int) -> Bool

  func execute(program: inout Program, address: Address, paramModes: ParameterModes) -> Address {
    let valueAddress = program[address + 1]
    let destinationAddress = program[address + 2]

    let value = paramModes.first.valueFrom(program: program, address: valueAddress)
    let destination = paramModes.second.valueFrom(program: program, address: destinationAddress)

    if operation(value) {
      return destination
    }

    return address + 3
  }
}

struct HaltInstruction: Instruction {
  let operation: (Program) -> Address

  func execute(program: inout Program, address: Address, paramModes: ParameterModes) -> Address {
    return operation(program)
  }
}

enum Command: Int {
  case addition = 1
  case multiplication = 2
  case input = 3
  case output = 4
  case jumpIfTrue = 5
  case jumpIfFalse = 6
  case lessThan = 7
  case equalTo = 8
  case halt = 99

  init(optcode: Int) {
    self.init(rawValue: optcode % 100)!
  }
}

func run(program: Program, input: Int) {
  var memory = program
  var optcodeAddress = 0

  while optcodeAddress < memory.endIndex {
    let optcode = memory[optcodeAddress]
    let command = Command(optcode: optcode)

    let instruction: Instruction

    switch command {
    case .addition:
      instruction = WriteInstruction(operation: +)
    case .multiplication:
      instruction = WriteInstruction(operation: *)
    case .input:
      instruction = InputInstruction(input: input)
    case .output:
      instruction = OutputInstruction(operation: { print($0) })
    case .jumpIfTrue:
      instruction = JumpInstruction(operation: { $0 != 0 })
    case .jumpIfFalse:
      instruction = JumpInstruction(operation: { $0 == 0 })
    case .lessThan:
      instruction = WriteInstruction(operation: { $0 < $1 ? 1 : 0 })
    case .equalTo:
      instruction = WriteInstruction(operation: { $0 == $1 ? 1 : 0 })
    case .halt:
      instruction = HaltInstruction(operation: { $0.endIndex })
    }

    let paramModes = ParameterModes(optcode: optcode)

    optcodeAddress = instruction.execute(
      program: &memory,
      address: optcodeAddress,
      paramModes: paramModes
    )
  }
}

let intcode = [3,225,1,225,6,6,1100,1,238,225,104,0,101,67,166,224,1001,224,-110,224,4,224,102,8,223,223,1001,224,4,224,1,224,223,223,2,62,66,224,101,-406,224,224,4,224,102,8,223,223,101,3,224,224,1,224,223,223,1101,76,51,225,1101,51,29,225,1102,57,14,225,1102,64,48,224,1001,224,-3072,224,4,224,102,8,223,223,1001,224,1,224,1,224,223,223,1001,217,90,224,1001,224,-101,224,4,224,1002,223,8,223,1001,224,2,224,1,223,224,223,1101,57,55,224,1001,224,-112,224,4,224,102,8,223,223,1001,224,7,224,1,223,224,223,1102,5,62,225,1102,49,68,225,102,40,140,224,101,-2720,224,224,4,224,1002,223,8,223,1001,224,4,224,1,223,224,223,1101,92,43,225,1101,93,21,225,1002,170,31,224,101,-651,224,224,4,224,102,8,223,223,101,4,224,224,1,223,224,223,1,136,57,224,1001,224,-138,224,4,224,102,8,223,223,101,2,224,224,1,223,224,223,1102,11,85,225,4,223,99,0,0,0,677,0,0,0,0,0,0,0,0,0,0,0,1105,0,99999,1105,227,247,1105,1,99999,1005,227,99999,1005,0,256,1105,1,99999,1106,227,99999,1106,0,265,1105,1,99999,1006,0,99999,1006,227,274,1105,1,99999,1105,1,280,1105,1,99999,1,225,225,225,1101,294,0,0,105,1,0,1105,1,99999,1106,0,300,1105,1,99999,1,225,225,225,1101,314,0,0,106,0,0,1105,1,99999,1107,226,226,224,102,2,223,223,1006,224,329,1001,223,1,223,1007,226,677,224,1002,223,2,223,1005,224,344,101,1,223,223,108,677,677,224,1002,223,2,223,1006,224,359,101,1,223,223,1008,226,226,224,1002,223,2,223,1005,224,374,1001,223,1,223,108,677,226,224,1002,223,2,223,1006,224,389,101,1,223,223,7,226,226,224,102,2,223,223,1006,224,404,101,1,223,223,7,677,226,224,1002,223,2,223,1005,224,419,101,1,223,223,107,226,226,224,102,2,223,223,1006,224,434,1001,223,1,223,1008,677,677,224,1002,223,2,223,1005,224,449,101,1,223,223,108,226,226,224,102,2,223,223,1005,224,464,1001,223,1,223,1108,226,677,224,1002,223,2,223,1005,224,479,1001,223,1,223,8,677,226,224,102,2,223,223,1006,224,494,1001,223,1,223,1108,677,677,224,102,2,223,223,1006,224,509,1001,223,1,223,1007,226,226,224,1002,223,2,223,1005,224,524,1001,223,1,223,7,226,677,224,1002,223,2,223,1005,224,539,1001,223,1,223,8,677,677,224,102,2,223,223,1005,224,554,1001,223,1,223,107,226,677,224,1002,223,2,223,1006,224,569,101,1,223,223,1107,226,677,224,102,2,223,223,1005,224,584,1001,223,1,223,1108,677,226,224,102,2,223,223,1006,224,599,1001,223,1,223,1008,677,226,224,102,2,223,223,1006,224,614,101,1,223,223,107,677,677,224,102,2,223,223,1006,224,629,1001,223,1,223,1107,677,226,224,1002,223,2,223,1005,224,644,101,1,223,223,8,226,677,224,102,2,223,223,1005,224,659,1001,223,1,223,1007,677,677,224,102,2,223,223,1005,224,674,1001,223,1,223,4,223,99,226]

print("Answer 1:")
run(program: intcode, input: 1)

print("Answer 2:")
run(program: intcode, input: 5)

let passwordRange = 153517...630395

let increasingDigits = passwordRange
  .lazy
  .map { (password: Int) in String(password).compactMap { $0.wholeNumberValue } }
  .filter { (digits: [Int]) in (0...4).filter { digits[$0] <= digits[$0 + 1] }.count == 5 }

let answer1 = increasingDigits
  .filter { (digits: [Int]) in (0...4).filter { digits[$0] == digits[$0 + 1] }.count > 0 }
  .count

print(answer1)

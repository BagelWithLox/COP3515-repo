// Mutable References

struct Fruit {
  apples: i32,
  bananas: i32,
}

fn increase_fruit(fruit: &mut Fruit) -> &Fruit {  
  fruit.apples *= 2;  
  fruit.bananas *= 3;  
  fruit
}

fn print_fruit(numFruit: &Fruit) {  
  println!("You have {} pieces of fruit", numFruit.apples+numFruit.bananas);
}
  
fn main() {  
  let mut fruit = Fruit {    
    apples: 10,    
    bananas: 5,  
  };  
  print_fruit(&fruit);  
  increase_fruit(&mut fruit);  
  print_fruit(&fruit);
}

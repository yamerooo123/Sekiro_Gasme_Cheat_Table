# sekiro shadows die twice cheat (CHEAT Table for cheat engine) - Unlimited ammo and HP

If you wanna find HP offset. Do memory scanning starting from "unknown initial value -> decrease value (get hit) -> repeat 3-4 times -> increase value (heal yourself) -> experiment on those returned memory addresses -> gen a pointermap -> profit!"

**Note:** unlimited ammo is easy, but HP is tricky. if you see a potential offset that contains an exact value of your character (e.g. 320). That is not an actual HP value. It is just a HP bar (UI). That's why when you try to manipulate the value, it keeps changing to the original value. You can check it through deassembly or select "Find out what access this value". You would see there are addresses that keep accessing it. However, you can also edit assembly code by navigating to that pointer address then in dissembly editor, replace source (rax) with any integer value (e.g. 500) you wish the UI to show. But your character will die anyway when taking hits.

**Precondition**
- don't use prayer beads to upgrade or else the cheat wouldn't work (yeah me is still a noob. I'm praciticing game hacking.)

<img width="783" height="707" alt="image" src="https://github.com/user-attachments/assets/b2efbb53-c411-4ae9-81b9-45cf91eb314c" />



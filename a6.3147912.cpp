#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>



using namespace std;

const int globalColumnWidth = 35;
const int annPerson = 200;   // Global integer variable for annual personal consumption
const int annSquare = 9;     // Global integer variable for annual square footage
const int annDevice = 200;   // Global integer variable for annual device consumption
const int annHotWater = 550; // annual average consumption per person with electric water heating

// Enum for power consumer usage frequency
enum class Use
{
    once = 1,
    daily,
    mo_fr, // Monday to Friday
    sa_su, // Saturday and Sunday
    weekly
};

// define an abstract class
class Device {
private:
    std::string description;
    Device* next;

public:
    explicit Device(Device* device = nullptr) : next(device) {}

    virtual ~Device() {
        std::cout << "Device " << description << " at address: " << this << " deleted." << std::endl;
    }

    std::string get_description() const {
        return description;
    }

    Device* get_next() const {
        return next;
    }

    void set_description(const std::string& desc) {
            description = desc;
        }
    
    void set_next(Device* device) {
        next = device;
    }

    virtual void input() {
        std::cout << "Enter the description for the device: ";
        std::cin >> description;
    }

    virtual void print(int integerParam, float floatParam) const = 0

    virtual float annual_kWh() const = 0

    virtual Device* get_a_copy() const = 0

    virtual std::string  get_string_for_file(char separator) const = 0
};



// Structure to store information about a power consumer
class consumer : public Device
{
private:
    static int total_number;
    Use use;
    float watt;
    
public:
    
    // standard constructor
    consumer(Device* device = nullptr)
           : Device(device) {
           total_number++;
       }
    
    // Deconstructor
    ~consumer() {
        
        std::cout << "Device " << get_description() << " at address: " << this << " deleted." << std::endl;
        total_number--;
        }
    // Static function to get total number of Consumer objects
        static int get_total_number() {
            return total_number;
        }
   // getter functions for all attributed
    //std::string getDescription() const { return description; }
    float getWatt() const { return watt; }
   
    Use getUse() const { return use; }
   // consumer* getNext() const { return next; }
    
 
    
    // setter functions for all attributes
   // void setDescription(std::string d){description=d;}
    void setWatt(float w){watt=w;}
 
    void setUse(Use u) { use = u; }
   // void setPointer(consumer* n){next=n;}
    
    // Input function for Consumer
        void input()  {
            Device::input();
            std::cout << "Enter the wattage: ";
            std::cin >> watt;
            
            int frequency;
            std::cout << "Enter frequency of use (0: once, 1: daily, 2: Monday to Friday, 3: Saturday and Sunday, 4: weekly): ";
            std::cin >> frequency;
            switch (frequency) {
                       case 0:
                           use = Use::once;
                           break;
                       case 1:
                           use = Use::daily;
                           break;
                       case 2:
                           use = Use::mo_fr;
                           break;
                       case 3:
                           use = Use::sa_su;
                           break;
                       case 4:
                           use = Use::weekly;
                           break;
                       default:
                           std::cerr << "Invalid frequency input. Using default frequency." << std::endl;
                   }
           
          
        }
    
    // Implementing pure virtual function from Device
     void print(int integerParam, float floatParam) const override {
         // Implementation for print function in consumer
     }

     // Implementing pure virtual function from Device
     float annual_kWh() const override {
         // Implementation for annual_kWh function in consumer
         return 0.0f;  // Replace with actual implementation
     }

     // Implementing pure virtual function from Device
     Device* get_a_copy() const override {
         // Implementation for get_a_copy function in consumer
         return new consumer(*this);  // Replace with actual implementation
     }

     // Implementing pure virtual function from Device
     std::string get_string_for_file(char separator) const override {
         // Implementation for get_string_for_file function in consumer
         return "";  // Replace with actual implementation
     }
};


int consumer::total_number = 0;

class Immobile : public consumer{
    
private:
    float hours;
    float wattStandby;
    
public:
    Immobile(Device* nextDevice = nullptr) : consumer(nextDevice) {}
    
     ~Immobile() {
            std::cout << "Immobile consumer " << get_description() << " at address " << this << " deleted" << std::endl;
        }
    
    // getter
    float getWattStandby() const { return wattStandby; }
    int getHours() const { return hours; }
    
    //setter
    void setwattStandby(float wattStand){wattStandby=wattStand;}
    void setHours(float hour){hours=hour;}
    
    //other functions
   // float annual_hours_of_use(Device* Consumer);
    float annual_hours_of_use(Device *device) const
{
    Immobile* immobileConsumer = dynamic_cast<Immobile*>(device);
        
    if (immobileConsumer == nullptr)
    {
        std::cerr << "Invalid consumer pointer." << std::endl;
        return 0.0f;
    }

    float annualHours = 0.0f;
    //std::cout<<Consumer->getUse()<<std::endl;
    switch (immobileConsumer->getUse())
    {
    case Use::once:
        annualHours = immobileConsumer->getHours();
        break;
    case Use::daily:
        annualHours = immobileConsumer->getHours() * 365.0f;
        break;
    case Use::mo_fr:
        annualHours = immobileConsumer->getHours() * 260.0f;
        break;
    case Use::sa_su:
        annualHours = immobileConsumer->getHours() * 104.0f;
        break;
    case Use::weekly:
        annualHours = immobileConsumer->getHours() * 52.0f;
        break;
    default:
        std::cerr << "Invalid frequency of use." << std::endl;
        break;
    }

    return annualHours;
}

    float  annual_hours_of_standby(Device *device) const
    {
        Immobile* immobileConsumer = dynamic_cast<Immobile*>(device);
        if (immobileConsumer == nullptr)
        {
            std::cerr << "Invalid consumer pointer." << std::endl;
            return 0.0f;
        }

        // Calculate standby hours in a year (8760 hours in a year - annual operating hours)
        return 8760.0f - annual_hours_of_use(immobileConsumer);
    }
    
    virtual float annual_kWh() const override
    {
        Device* device=nullptr;
        Immobile* immobileConsumer = dynamic_cast<Immobile*>(device);
        if (immobileConsumer == nullptr)
        {
            std::cerr << "Invalid consumer pointer." << std::endl;
            return 0.0f;
        }

        float operatingConsumption = annual_hours_of_use(immobileConsumer) * immobileConsumer->getWatt();
        float standbyConsumption = annual_hours_of_standby(immobileConsumer) * immobileConsumer->getWattStandby();

        // Total consumption in watt hours
        float totalConsumption = operatingConsumption + standbyConsumption;

        // Convert to kilowatt hours (kWh)
        return totalConsumption / 1000.0f;
    }

    void  input()
    {
        consumer:: input();
        // Function to input the frequency of power consumer usage
        std::cout << "Input standby" << std::endl;
        std::cin>>wattStandby;
        std::cout << "Input hours" << std::endl;
        std::cin>>hours;
        
 

       
    }
    

    virtual void print(int number, float powerPrice) const override {
        std::cout << std::right << std::setw(globalColumnWidth) << number << "Immobile: " << std::left << get_description();
        std::cout << "(At address:" << this->get_description() << ")" << std::endl;
        
        
        std::cout << std::right << std::setw(globalColumnWidth) << "Power Consumption: " << std::left << getWatt() << " W" << std::endl;
        
         std::cout << "   Annual consumption: " << annual_kWh() << " kWh" << std::endl;
        std::cout << std::right << std::setw(globalColumnWidth) << "Power Consumption Standby: " << std::left << getWattStandby() << " W" << std::endl;
         std::cout << "   Costs: " << annual_kWh() * powerPrice/1000 << " EUR" << std::endl;
     }
    
    virtual Device* get_a_copy() const override {
        // Create a new object of the class Immobile on the heap
        Immobile* copy = new Immobile;

        // Copy all attribute values
        *copy = *this;

        // Return the address of the new object
        return copy;
    }
    
    std::string virtual get_string_for_file(char separator) const override {
        // Create a stringstream to build the string
        std::ostringstream oss;

        // Concatenate the string starting with "Immobile"
        oss << "Immobile" << separator;

        // Append values of attributes (except next)
        oss << get_description() << separator;
        oss << getWatt() << separator;
        oss << static_cast<int>(getUse()) << separator;  // Assuming get_use returns Use enum
        oss << getHours() << separator;
        oss << getWattStandby() << separator;

     

        // Return the constructed string
        return oss.str();
    }
};

class Mobile : public consumer {
private:
    float km;  // Kilometres driven by the mobile device

public:
    // Constructors
    Mobile(Device* nextDevice = nullptr) : consumer(nextDevice){}

    // Destructor
    ~Mobile() override {
        std::cout << "Mobile: " << get_description() << " at address " << this << " deleted." << std::endl;
    }

    // Getter and setter for km
    float getKilometers() const { return km; }
    void setKilometers(float kilometers) { km = kilometers; }

    // Overridden input function
    void input() override {
        consumer::input();  // Call input function of the base class

        std::cout << "Enter the kilometres driven: ";
        std::cin >> km;
    }
    
    virtual float annual_kWh() const override {
        // Assuming there's a constant for kilowatt hours consumed per 100 km
      
         float kwhUse=consumer::getWatt();
       

        // Calculate annual power consumption
        float annualConsumption = 0.0f;
        switch (getUse()) {
            case Use::once:
                annualConsumption = (getKilometers() / 100.0f) * kwhUse;  // Convert km to 100 km and calculate
                break;
            case Use::daily:
                annualConsumption = 365.0f * (getKilometers() / 100.0f) * kwhUse;
                break;
            case Use::mo_fr:
                annualConsumption = 260.0f * (getKilometers() / 100.0f) * kwhUse;
                break;
            case Use::sa_su:
                annualConsumption = 104.0f * (getKilometers() / 100.0f) * kwhUse;
                break;
            case Use::weekly:
                annualConsumption = 52.0f * (getKilometers() / 100.0f) * kwhUse;
                break;
        }

        return annualConsumption;
    }

   virtual void print(int number, float powerPrice) const override {
        std::cout << number << ". Mobile " << get_description() << " at address: " << this << std::endl;
        std::cout << "   Consumption per 100 km: " << getWatt() / 1000.0f << " kW" << std::endl;  // Convert watt to kilowatts
        std::cout << "   Kilometres driven: " << getKilometers() << " km" << std::endl;
        std::cout << "   Annual consumption: " << annual_kWh()/1000 << " kWh" << std::endl;
        std::cout << "   Costs: " << (annual_kWh() * powerPrice)/1000<< " EUR" << std::endl;
    }



    // Overridden function to get a copy of the object
   virtual Device* get_a_copy() const override {
        // Create a new object of the class Mobile on the heap
        Mobile* copy = new Mobile;

        // Copy all attribute values
        *copy = *this;

        // Return the address of the new object
        return copy;
    }

    std::string virtual get_string_for_file(char separator) const override {
        // Create a stringstream to build the string
        std::ostringstream oss;

        // Concatenate the string starting with "Mobile"
        oss << "Mobile" << separator;

        // Append values of attributes (except next)
        oss << get_description() << separator;
        oss << getWatt() << separator;
        oss << static_cast<int>(getUse()) << separator;  // Assuming get_use returns Use enum
        oss << getKilometers() << separator;

        // Return the constructed string
        return oss.str();
    }

};

class Producer : public Device {
private:
    static int totalNumber;
    
public:
    // Standard constructor
  /*  Producer(Device* device = nullptr)
        : Device(device), use(Use::once), watt(0.0f) {
        total_number++;
    }*/
    
    Producer(Device* device = nullptr)
        : Device(device){
            totalNumber++;
    }
     // Destructor
    virtual ~Producer() override {
        std::cout << "Producer " << get_description() << " at address: " << this << " deleted." << std::endl;
        totalNumber--;
    }

    static int TotalNumber() {
        return totalNumber;
    }
    // Input function for Producer
    void input() override {
        Device::input();
    }
};

int Producer::totalNumber=0;

class Solar : public Producer {
private:
    float watt_peak;
    int year;

public:
    // Overloaded constructor
    Solar(Device* device = nullptr) : Producer(device){}

    // Virtual destructor
    virtual ~Solar() {
        std::cout << "Solar:"<<get_description()<<"at address" << this << " deleted" << std::endl;
    }

    // Getter and setter functions for watt_peak
    float get_watt_peak() const {
        return watt_peak;
    }

    void set_watt_peak(float wattPeak) {
        watt_peak = wattPeak;
    }

    // Getter and setter functions for year
    int get_year() const {
        return year;
    }

    void set_year(int installationYear) {
        year = installationYear;
    }

    // Input function
    void input() override {
        Producer::input();  // Call superclass input function
        std::cout << "Enter the installed power of the solar plant (watt peak): ";
        std::cin >> watt_peak;
        std::cout << "Enter the year of installation: ";
        std::cin >> year;
    }

    // Annual kilowatt hours generated by the solar plant
       float annual_kWh() const override {
           // Calculate the age of the solar plant in years
           int currentYear = 2024; // Update with the current year
           int age = currentYear - year;

           // Calculate the reduction in power output due to ageing (0.5% reduction per year)
           float reductionFactor = pow(0.995, age);

           // Calculate the annual kilowatt hours generated
           float generated_kWh = watt_peak * reductionFactor;

           // Return the value with a negative sign
           return -generated_kWh;
       }

        void print(int number, float powerPrice) const override {
            
            std::cout << number << ". Solar " << get_description() << " at address: " << this << std::endl;
            std::cout << " (this: " << this << ")"<<std::endl;
            // Output additional information specific to Solar plant
            std::cout << " Installed Power: " << watt_peak << " Wp\n";
           

            // Calculate the age of the solar plant in years
            int currentYear = 2024; // Update with the current year
            int age = currentYear - year;

            // Output age information
            std::cout << " Age of Solar Cells: " << age << " years\n";

            // Output the annual kilowatt hours generated
            float annualProduction = annual_kWh();
            std::cout << " Annual Production: " << annualProduction << " kWh\n";

            // Calculate the costs saved
            float costSaved = annualProduction * powerPrice; // Make it positive for output
            std::cout << " Annual Savings: " << costSaved << " EUR\n";

        }
    
   virtual Device* get_a_copy() const override {
           // Create a new object of the class Solar on the heap
           Solar* copy = new Solar;

           // Copy all attribute values
           *copy = *this;

           // Return the address of the new object
           return copy;
       }
    
    std::string virtual get_string_for_file(char separator) const override {
        // Create a stringstream to build the string
        std::ostringstream oss;

        // Concatenate the string starting with "Solar"
        oss << "Solar" << separator;

        // Append values of attributes (except next)
        oss << get_description() << separator;
        oss << get_watt_peak() << separator;
        oss << get_year() << separator;

        // Return the constructed string
        return oss.str();
    }
};



// Structure to store information about a household
class Household
{
private:
    int numPersons;
    int numSquareMeter;
    Device* devices; // Pointer from the consumer structure to the household structure
    char hotWaterChoice; // I had no better idea but to add this to the structure
    int houseNumber;     //  I had no better idea but to add this to the structure
    float kwH;//price for each killo watt
    std::string powerSupplier;
    
public:
    
    //constructor
    Household() {
        this->devices=nullptr;
    }
    
    //Deconstructor
    ~Household() {
        // Output information before deletion
        std::cout << "Deleting Household object at address: " << this << std::endl;
        
        // Delete all power consumers
        Device *toDelete;
        while (devices != nullptr) {
            toDelete = devices;
            devices = devices->get_next();
            delete toDelete;
        }
    }
    
    // Getter functions for all attributes
    int getHouseNumber() const { return houseNumber; }
    int getNumSquareMeter() const { return numSquareMeter; }
    int getNumPersons() const { return numPersons; }
    char getHotWaterChoice() const { return hotWaterChoice; }
    float getKwH() const { return kwH; }
    std::string getPowerSupplier() const { return powerSupplier; }
    Device* getDevices() const { return devices; }
    
    // Setter functions for all attributes
    void setHouseNumber(int number) { houseNumber = number; }
    void setNumSquareMeter(int squareMeter) { numSquareMeter = squareMeter; }
    void setNumPersons(int persons) { numPersons = persons; }
    void setHotWaterChoice(char choice) { hotWaterChoice = choice; }
    void setKwH(float kwh) { kwH = kwh; }
    void setPowerSupplier(const std::string& supplier) { powerSupplier = supplier; }
    void setDevices(Device* devs) { devices = devs; }
    /* int getTotalConsumers() {
     return this->total;
     }
     */
    
    // Function to add a device to the household
    void add_device_to_household(Household* household, Device* newDevice)
    {
        if (household == nullptr || newDevice == nullptr)
        {
            std::cerr << "Invalid household or consumer." << std::endl;
            return;
        }
        
        // Add the new consumer to the front of the linked list
        newDevice->set_next(household->getDevices());
        household->setDevices(newDevice);
        std::cout<<"the device is added successfully"<<std::endl;
        
    }
    
    Device* move_up(Device* head, int k) {
            if (head == nullptr || k < 2) {
                std::cerr << "Invalid device list or position." << std::endl;
                return head;
            }
            
            if (k == 2) {
                // Special case: Move the second element to the front
                Device* toMove = head->get_next();
                head->set_next(toMove->get_next());
                toMove->set_next(head);
                return head;
            }
            
            Device* current = head;
            int i = 1;
            
            while (current->get_next() != nullptr && i < k - 2) {
                current = current->get_next();
                i++;
            }
            
            if (current == nullptr || current->get_next() == nullptr) {
                std::cerr << "Position k is out of bounds." << std::endl;
                return head;
            }
            
            Device* prev = current;
            Device* toMove=current;
            toMove = current->get_next()->get_next();
            current=current->get_next();
            Device* temp = toMove->get_next();
            prev->set_next(toMove);
            toMove->set_next(current);
            current->set_next(temp);
            return head;
        }

    
    void  input_household(Household *myHousehold)
    {
        if (myHousehold == nullptr)
        {
            std::cerr << "Invalid household pointer." << std::endl;
            return;
        }
        
        std::cout << "the number of household:" << std::endl;
        std::cin >> myHousehold->houseNumber;
        
        // Read the number of square meters
        std::cout << "How many square meters does the household have? ";
        std::cin >> myHousehold->numSquareMeter;
        
        // Read the number of people
        std::cout << "How many persons live in this household? " << std::endl;
        ;
        std::cin >> myHousehold->numPersons;
        
        // Read whether hot water is produced electrically or not
        
        std::cout << "is hot water heated using electricity? (y or n) " << std::endl;
        std::cin >> myHousehold->hotWaterChoice;
        
        myHousehold->devices = nullptr; // in order to make a empty list for list of consumers for further step
        
        std::cout << "Enter the power price in EUR: ";
        std::cin >> myHousehold->kwH;
        
        std::cout << "Enter the power supplier: ";
        std::cin >> myHousehold->powerSupplier;
        
    }
    
    
    
    // Function to copy devices from source to destination household
    Household* copy_devices(const Household* sourceHousehold, Household* destinationHousehold) {
        if (sourceHousehold == nullptr || destinationHousehold == nullptr) {
            std::cerr << "Invalid source or destination household pointers." << std::endl;
            return destinationHousehold;
        }
        
        // Create pointers for iterating through the source and destination lists
        Device* sourceCurrent = sourceHousehold->getDevices();
        Device* destinationCurrent = destinationHousehold->getDevices();
        Device* destinationPrev = nullptr;
        
        // Traverse to the end of the destination devices list
        while (destinationCurrent != nullptr) {
            destinationPrev = destinationCurrent;
            destinationCurrent = destinationCurrent->get_next();
        }
        
        // Iterate through the source list and append devices to the destination list
        while (sourceCurrent != nullptr) {
            // Use the virtual function to get a copy of the device
            Device* newDevice = sourceCurrent->get_a_copy();
            
            // Append the new device to the destination list
            if (destinationPrev == nullptr) {
                destinationHousehold->setDevices(newDevice);
            } else {
                destinationPrev->set_next(newDevice);
            }
            
            // Move to the next device in the source list
            sourceCurrent = sourceCurrent->get_next();
            destinationPrev = newDevice;
        }
        
        return destinationHousehold;
    }
};
    
    class Address {
    private:
        std::string street;
        std::string no;
        std::string zip;
        std::string city;
        
    public:
        // Public constructor with default values
        Address(std::string street = "", std::string no = "", std::string zip = "", std::string city = "")
        : street(street), no(no), zip(zip), city(city) {}
        
        // deconstructor
        ~Address() {
            std::cout << "Address: " << street << " " << no << ", " << zip << " " << city << " is deleted. (this: " << this << ")" << std::endl;
        }
        // Getter methods for each attribute
        std::string getStreet() const { return street; }
        std::string getNo() const { return no; }
        std::string getZip() const { return zip; }
        std::string getCity() const { return city; }
        
        
        //set functions
        void setStreet(std::string nameStreet){street=nameStreet;}
        void setNum(std::string houseNumber){no=houseNumber;}
        void setZip(std::string numberZip){zip=numberZip;}
        void setcity(std::string cityName){city=cityName;}
        
        // Method to return the address as a single string
        std::string to_string() const {
            return street + " " + no + ", " + zip + " " + city;
        }
    };
    
    // Assume that the Household class is defined as described earlier
    
    class House {
        
    private:
        Address address;
        int number_of_households;
        Household** households;
        
        
    public:
        // Constructor
        House(int num, const Address& addr) : number_of_households(num), address(addr) {
            households = new Household*[number_of_households];
            for (int i = 0; i < number_of_households; ++i) {
                households[i] = nullptr;
            }
        }
        
        // Destructor
        ~House() {
            for (int i = 0; i < number_of_households; ++i) {
                delete households[i];
            }
            delete[] households;
        }
        
        // Getter method to return a pointer to the n-th household
        Household* get_household(int n) const {
            if (n >= 0 && n < number_of_households) {
                return households[n];
            }
            return nullptr;
        }
        
        int getNumberOfHouseholds(){return number_of_households;}
        
        
        // Overloaded unary operator[] with the same functionality as the get_household method
        Household* operator[](int n) const {
            return get_household(n);
        }
        
        // Setter method to save the household as the n-th household
        void set_household(Household* h, int n) {
            if (n >= 0 && n < number_of_households) {
                delete households[n]; // Delete the existing household if any
                households[n] = h;
            }
        }
        
        
        void setNumberOfHouseholds(int numHouseholds){ number_of_households=numHouseholds;}
        
        
        // Output all households in a house including the value of this to the standard character output stream
        
        void  print_household(House *house)
              {
                  if (house == nullptr)
                  {
                      std::cerr << "Invalid household pointer." << std::endl;
                      return;
                  }
                  std::cout << "===============================================================" << std::endl;
                  std::cout << std::right << std::setw(globalColumnWidth) << "House: " << std::left;
                  std::cout << "===============================================================" << std::endl;
                  std::cout <<std::right << std::setw(globalColumnWidth)<< "(This at address : " << house << ")" << std::endl;
                 // std::cout<<std::right << std::setw(globalColumnWidth) << "address: " << house->address()<<std::endl;
                  std::cout<<std::right << std::setw(globalColumnWidth) << "Number of Households: " << house->getNumberOfHouseholds()<<std::endl;
                  std::cout << std::right << std::setw(globalColumnWidth) << "Number of consumers:" <<consumer::get_total_number()+ Producer::TotalNumber()<<std::endl;
                  
                  std::cout << "===============================================================" << std::endl;
                  std::cout<<std::endl;
                  
                  for (int i = 0; i < house->getNumberOfHouseholds(); ++i) {
                      
                      if(house->get_household(i)!=nullptr){
                          std::cout << "H O U S E H O L D  N O " << house->get_household(i)->getHouseNumber() << "   P O W E R   C O N S U M P T I O N" << std::endl;
                          std::cout << "---------------------------------------------------------------------" << std::endl;
                          std::cout << std::right << std::setw(globalColumnWidth) << "This: " << std::left << house->get_household(i) << ")" << std::endl;
                          std::cout << "The price for one kWh in ct/kWh is: " << std::fixed << std::setprecision(2) <<   house->get_household(i)->getKwH() * 100 << std::endl;
                          std::cout << std::right << std::setw(globalColumnWidth) << "Power supplier: " << std::left <<  house->get_household(i)->getPowerSupplier()<<std::endl;
                          
                          std::cout << std::right << std::setw(globalColumnWidth) << "Square Meters: " << std::left << house->get_household(i)->getNumSquareMeter() << " qm" << std::endl;
                          std::cout << std::right << std::setw(globalColumnWidth) << "Number of Persons: " << std::left << house->get_household(i)->getNumPersons() << std::endl;
                          std::cout << "water heated using electricity:" << house->get_household(i)->getHotWaterChoice() << std::endl;
                          
                          std::cout << std::right << std::setw(globalColumnWidth) << "List of Consumers:" << std::endl;
                          std::cout << "---------------------------------------------------------------------" << std::endl;
                          // Loop through the list of power consumers in the household
                          float annConsumption;
                          float annCosts;
                          float powerConsSquMeter;
                          float powerConsPersons;
                          float consumerConsumption;
                          float consumerCosts;
                          
                          int index = 1;
                          
                          
                          if (house->get_household(i)->getHotWaterChoice()== 'n')
                          { // modify this part accordingly
                              powerConsSquMeter = house->get_household(i)->getNumSquareMeter() * annSquare;
                              powerConsPersons = house->get_household(i)->getNumPersons() * annPerson;
                              annConsumption = powerConsSquMeter + powerConsPersons;
                              annCosts = annConsumption * house->get_household(i)->getKwH();
                          }
                          
                          else if (house->get_household(i)->getHotWaterChoice() == 'y')
                          {
                              
                              powerConsSquMeter = house->get_household(i)->getNumSquareMeter() * annSquare;
                              powerConsPersons = house->get_household(i)->getNumPersons() * annHotWater;
                              annConsumption = powerConsSquMeter + powerConsPersons;
                              annCosts = annConsumption * house->get_household(i)->getKwH();
                          }
                          
                          Device *currentDevice = house->get_household(i)->getDevices();
                          
                          
        while (currentDevice != nullptr)
        {
             currentDevice->print(index, house->get_household(i)->getKwH());
                              
             if (Immobile* immobileConsumer = dynamic_cast<Immobile*>(currentDevice)) {
                 consumerConsumption = immobileConsumer->annual_kWh();
                 consumerCosts = consumerConsumption * house->get_household(i)->getKwH()/1000;
                 
                 annConsumption += consumerConsumption;
                 annCosts += consumerCosts;
                 //std::cout<<"Debugging:value for Immobile:"<<annConsumption<<std::endl;
                // std::cout<<"Debugging:value for Immobile:"<<annCosts<<std::endl;
                }
             else if(Mobile* mobileConsumer = dynamic_cast<Mobile*>(currentDevice)){
                 consumerConsumption=mobileConsumer->annual_kWh()/1000;
                 consumerCosts = mobileConsumer->annual_kWh() * house->get_household(i)->getKwH()/1000;
                 annConsumption += consumerConsumption;
                 annCosts += consumerCosts;
                 std::cout<<"Debugging:value for Mobile:"<<annConsumption<<std::endl;
                 std::cout<<"Debugging:value for Mobile:"<<annCosts<<std::endl;
               }
             else if (Solar* solarConsumer = dynamic_cast<Solar*>(currentDevice)){
                 float annualProduction =solarConsumer-> annual_kWh();
                 float costSaved = annualProduction * house->get_household(i)->getKwH();
                 annConsumption += annualProduction;
                 annCosts += costSaved;
               //  std::cout<<"Debugging:value for Solar:"<<annConsumption<<std::endl;
                // std::cout<<"Debugging:value for Solar:"<<annCosts<<std::endl;
             }
                              
            currentDevice = currentDevice->get_next();
            index++;
            }
        std::cout << "..............................................................................................." << std::endl;
                                         
        std::cout << "power consumption square meters: : " << std::fixed <<std::setprecision(2) << powerConsSquMeter << " kWh" << std::endl;
        std::cout << "power consumption all persons: : " << std::fixed << std::setprecision(2) << powerConsPersons << " kWh" << std::endl;
        std::cout << "Total annual power consumption : " << std::fixed <<std::setprecision(2) << annConsumption << " kWh" << std::endl;
        std::cout << "Total annual power costs: " << std::fixed << std::setprecision(2) << annCosts << " EUR" << std::endl;
                        
                      }
                  }
              }
        void writeDataToFile(const std::string& fileName, char separator, House* house) {
            // Open the file for writing
            std::ofstream outFile(fileName);
            
            // Check if the file is open
            if (!outFile.is_open()) {
                std::cerr << "Error: Unable to open file " << fileName << " for writing." << std::endl;
                return;
            }
            
            std::cout << "The target file is opened" << std::endl;
            
            // Write the version number and other information
            outFile << "A6" << separator << house->getNumberOfHouseholds() << separator
            << house->address.getStreet() << separator
            << house->address.getNo() << separator
            << house->address.getZip() << separator
            << house->address.getCity() << std::endl;
            
            // Write household and consumer data
            for (int i = 0; i < house->getNumberOfHouseholds(); ++i) {
                Household* currentHousehold = house->get_household(i);
                if (currentHousehold != nullptr) {
                    // Write household information
                    outFile << "household" << separator
                    << currentHousehold->getHouseNumber() << separator
                    << (currentHousehold->getHotWaterChoice() == 'y' ? "true" : "false") << separator
                    << currentHousehold->getNumPersons() << separator
                    << currentHousehold->getNumSquareMeter() << separator
                    << currentHousehold->getKwH() << separator
                    << currentHousehold->getPowerSupplier() << std::endl;
                    
                    // Write consumer information using get_string_for_file
                    Device* currentConsumer = currentHousehold->getDevices();
                    while (currentConsumer != nullptr) {
                        outFile << currentConsumer->get_string_for_file(separator) << std::endl;
                        currentConsumer = currentConsumer->get_next();
                    }
                }
            }
            
            // Close the file
            outFile.close();
            
            // Print the path of the created file
            std::cout << "File created successfully: " << fileName << std::endl;
        }
        
        
        
        void readDataFromFileTest(const std::string fileName, char separator, House* houses) {
            std::ifstream inFile(fileName);
            
            if (!inFile.is_open()) {
                std::cerr << "Error: Unable to open file " << fileName << " for reading." << std::endl;
                return;
            }
            
            std::cout << "The target file is opened" << std::endl;
            std::string line;
            
            int i = 0;
            //consumer* newConsumer = nullptr;
            Household* h = nullptr;
            
            while (std::getline(inFile, line)) {
                std::istringstream s(line);
                std::string type;
                std::getline(s, type, separator);
                
                if (type == "A6") {
                    std::string maxHouseholdsString;
                    std::getline(s, maxHouseholdsString, separator);
                    int maxHousesParsed = std::stoi(maxHouseholdsString);
                    
                    std::string stringStreet, stringhNo, stringZip, stringCity;
                    int No,Zip;
                    std::getline(s, stringStreet, separator);
                    std::getline(s, stringhNo, separator);
                     No= std::stoi(stringhNo);
                    std::getline(s, stringZip, separator);
                     Zip = std::stoi(stringZip);
                    std::getline(s, stringCity, separator);
                    
                    houses->setNumberOfHouseholds(maxHousesParsed);
                    houses->address.setStreet(stringStreet);
                    houses->address.setNum(stringhNo);
                    houses->address.setZip(stringZip);
                    houses->address.setcity(stringCity);
                } else if (type == "Household") {
                    h = new Household;
                    std::string houseIndexString, hotWaterProduced, householdPeopleString, squareMetersString, filePowerPricestring, filePowerSupllier;
                    int houseIndexParsed, householdPeopleParsed, squareMeterParsed;
                    float filePowerPrice;
                    
                    std::getline(s, houseIndexString, separator);
                    std::getline(s, hotWaterProduced, separator);
                    std::getline(s, householdPeopleString, separator);
                    std::getline(s, squareMetersString, separator);
                    std::getline(s, filePowerPricestring, separator);
                    std::getline(s, filePowerSupllier, separator);
                    
                    houseIndexParsed = std::stoi(houseIndexString);
                    householdPeopleParsed = std::stoi(householdPeopleString);
                    squareMeterParsed = std::stoi(squareMetersString);
                    filePowerPrice = std::stof(filePowerPricestring);
                    
                    h->setHouseNumber(houseIndexParsed);
                    h->setNumPersons(householdPeopleParsed);
                    h->setNumSquareMeter(squareMeterParsed);
                    
                    if (hotWaterProduced == "true") {
                        h->setHotWaterChoice('y');
                    } else {
                        h->setHotWaterChoice('n');
                    }
                    
                    h->setKwH(filePowerPrice);
                    h->setPowerSupplier(filePowerSupllier);
                    
                    houses->set_household(h, houseIndexParsed);
                } else if (type == "Mobile") {
                    
                    Mobile* mobileDevice = new Mobile;
                    
                    std::string mobileDescription, mobileWatt, mobileFrequencyString,  mobileKilloMeter;
                    float mobilewatt;
                    float mobileKillometer;
                    
                    std::getline(s, mobileDescription, separator);
                    std::getline(s, mobileKilloMeter, separator);
                    mobileKillometer=std::stoi(mobileKilloMeter);
                    std::getline(s, mobileFrequencyString, separator);
                    std::getline(s, mobileWatt, separator);
                    mobilewatt=std::stoi(mobileWatt);
                    
                    
                    mobileDevice->set_description(mobileDescription);
                    mobileDevice->setKilometers(mobileKillometer);
                    mobileDevice->setWatt(mobilewatt);
                    
                    if (mobileFrequencyString == "once") {
                        mobileDevice->setUse(Use::once);
                    } else if (mobileFrequencyString == "daily") {
                        mobileDevice->setUse(Use::daily);
                    } else if (mobileFrequencyString == "Monday to Friday") {
                        mobileDevice->setUse(Use::mo_fr);
                    } else if (mobileFrequencyString == "Saturday and Sunday") {
                        mobileDevice->setUse(Use::sa_su);
                    } else if (mobileFrequencyString == "weekly") {
                        mobileDevice->setUse(Use::weekly);
                    }
                    
                    h->add_device_to_household(h, mobileDevice);
                }
                else if (type == "Immobile") {
                    std::string immobileDescription, operatingHoursString, useFrequencyString, powerConsumptionString, standbyConsumptionString;
                    int operatingHoursParsed;
                    float powerConsumptionParsed, standbyConsumptionParsed;
                    
                    std::getline(s, immobileDescription, separator);
                    std::getline(s, operatingHoursString, separator);
                    std::getline(s, useFrequencyString, separator);
                    std::getline(s, powerConsumptionString, separator);
                    std::getline(s, standbyConsumptionString, separator);
                    
                    operatingHoursParsed = std::stoi(operatingHoursString);
                    powerConsumptionParsed = std::stof(powerConsumptionString);
                    standbyConsumptionParsed = std::stof(standbyConsumptionString);
                    
                    Immobile* immobileDevice = new Immobile;
                    
                    immobileDevice->set_description(immobileDescription);
                    immobileDevice->setHours(operatingHoursParsed);
                    immobileDevice->setWatt(powerConsumptionParsed);
                    immobileDevice->setwattStandby(standbyConsumptionParsed);
                    
                    if (useFrequencyString == "once") {
                        immobileDevice->setUse(Use::once);
                    } else if (useFrequencyString == "daily") {
                        immobileDevice->setUse(Use::daily);
                    } else if (useFrequencyString == "Monday to Friday") {
                        immobileDevice->setUse(Use::mo_fr);
                    } else if (useFrequencyString == "Saturday and Sunday") {
                        immobileDevice->setUse(Use::sa_su);
                    } else if (useFrequencyString == "weekly") {
                        immobileDevice->setUse(Use::weekly);
                    }
                    h->add_device_to_household(h, immobileDevice);
                }
                
                else if (type == "Solar") {
                    std::string solarDescription, yearString, peakWattString;
                    int yearParsed;
                    float peakWattParsed;
                    
                    std::getline(s, solarDescription, separator);
                    std::getline(s, yearString, separator);
                    yearParsed = std::stoi(yearString);
                    std::getline(s, peakWattString, separator);
                    peakWattParsed = std::stoi(peakWattString);
                    
                    Solar* solarDevice=new Solar;
                    solarDevice->set_description(solarDescription);
                    solarDevice->set_year(yearParsed);
                    solarDevice->set_watt_peak(peakWattParsed);
                    h->add_device_to_household(h, solarDevice);
                    
                }
                
                else {
                    std::cout<<"Error: no info is parsed"<<std::endl;
                }
                
                ++i;
            }
            
            inFile.close(); // Close the file after processing
            std::cout << "The target file is closed" << std::endl;
        }
        
        
    };
    
    
    
    int main()
    {
        std::cout << "CALCULATION OF AVERAGE POWER COSTS FOR MULTIPLE HOUSEHOLDS: Class Version" << std::endl;
        
        // Declare a pointer to House
        House* house = nullptr;
        
        
        
        char separator;
        string fileName;
        
        
        char choice;
        Device *newConsumer = nullptr;
        
        Household* myHousehold = nullptr;
        Household* targetHousehold = nullptr;
        float price;
        int indexfromFile;
        Use u;
        while (true)
        {
            
            std::cout << ".................................................................." << std::endl;
            std::cout << "Menu:" << std::endl;
            std::cout << "Select an option from the following list:" << std::endl;
            std::cout << "q: Quit program" << std::endl;
            std::cout << "u: Move up power consumer" << std::endl;
            std::cout << "p: Print household(consumers)" << std::endl;
            std::cout << "a: Print all households" << std::endl;
            std::cout << "n: New household" << std::endl;
            std::cout << "c: Copy all consumers" << std::endl;
            std::cout << "r: read data from a file" << std::endl;
            std::cout << "w: write data into file" << std::endl;
            std::cout << "h: House initialization" << std::endl;
            std::cout << "d: delete house" << std::endl;
            std::cout << "i: Input immobile consumer" << std::endl;
            std::cout << "m: Input mobile consumer" << std::endl;
            std::cout << "s: Input solar producer" << std::endl;
            std::cout << "Enter your choice: ";
            std::cin >> choice;
            
            
            // bool cityExists = false;
            switch (choice)
            {
                case 'q':
                    delete house;  // Release the memory allocated for the House object
                    return 0;
                    
                    
                case 'm':
                {
                    int targetHouseholdNumber;
                    std::cout << "Enter the household number to add consumers to: ";
                    std::cin >> targetHouseholdNumber;
                    
                    // Find the target household in the array
                    for (int i = 0; i < house->getNumberOfHouseholds(); ++i)
                    {
                        if (house->get_household(i) != nullptr && house->get_household(i)->getHouseNumber() == targetHouseholdNumber)
                        {
                            targetHousehold = house->get_household(i);
                            break;
                        }
                    }
                    
                    if (targetHousehold != nullptr)
                    {
                        // Create a new Immobile object on the heap
                        Mobile* newMobile = new Mobile;
                        
                        newMobile->input();
                        
                        // Add the new Immobile consumer to the household
                        targetHousehold->add_device_to_household(targetHousehold, newMobile);
                    }
                }
                    
                    break;
                    
                case 's':
                    int targetHouseholdNumber;
                    std::cout << "Enter the household number to add consumers to: ";
                    std::cin >> targetHouseholdNumber;
                    
                    // Find the target household in the array
                    for (int i = 0; i < house->getNumberOfHouseholds(); ++i)
                    {
                        if (house->get_household(i) != nullptr && house->get_household(i)->getHouseNumber() == targetHouseholdNumber)
                        {
                            targetHousehold = house->get_household(i);
                            break;
                        }
                    }
                    
                    if (targetHousehold != nullptr)
                    {
                        // Create a new Immobile object on the heap
                        Solar* solar = new Solar;
                        
                        std::cout << "Enter the description: " << std::endl;
                        std::string newDescription;
                        std::cin >> newDescription;
                        solar->set_description(newDescription);
                        
                        std::cout << "how many watt peak have been installed? ";
                        float peakWatt;
                        std::cin >> peakWatt;
                        solar->set_watt_peak(peakWatt);
                        
                        std::cout << "in which year the solar modules are installed? ";
                        int year;
                        std::cin >> year;
                        solar->set_year(year);
                        
                        // Add the new Immobile consumer to the household
                        targetHousehold->add_device_to_household(targetHousehold, solar);
                 
                        
                    }
                    break;
                    
                case 'd':
                    delete house;  // Release the memory allocated for the House object
                    house = nullptr;  // Set the house pointer to nullptr
                    std::cout << "House deleted." << std::endl;
                    break;
                    
                case 'i':
                {
                    int targetHouseholdNumber;
                    std::cout << "Enter the household number to add consumers to: ";
                    std::cin >> targetHouseholdNumber;
                    
                    // Find the target household in the array
                    for (int i = 0; i < house->getNumberOfHouseholds(); ++i)
                    {
                        if (house->get_household(i) != nullptr && house->get_household(i)->getHouseNumber() == targetHouseholdNumber)
                        {
                            targetHousehold = house->get_household(i);
                            break;
                        }
                    }
                    
                    if (targetHousehold != nullptr)
                    {
                        // Create a new Immobile object on the heap
                        Immobile* newImmobile = new Immobile;
                        
                       newImmobile->input();
                        //newImmobile->setUse();
                        // Set additional attributes specific to Immobile
                        std::cout << "How many hours will it be operating then? ";
                        float newHours;
                        std::cin >> newHours;
                        newImmobile->setHours(newHours);
                        
                        // Set Immobile-specific attributes or call Immobile-specific input functions if needed
                        
                        // Add the new Immobile consumer to the household
                        targetHousehold->add_device_to_household(targetHousehold, newImmobile);
                    }
                }
                    break;
                    
                case 'u':
                    // Move a consumer up in the list
                    int position;
                    int target;
                    std::cout << "number of household?"<<std::endl;
                    std::cin >> target;
                    
                    for (int i = 0; i < house->getNumberOfHouseholds(); ++i)
                    {
                        if (house->get_household(i) != nullptr && house->get_household(i)->getHouseNumber() == target)
                        {
                            std::cout << "Enter the number of the consumer to move up: ";
                            std::cin >> position;
                            Device * firstConsumerintargetHousehold=house->get_household(i)->getDevices();
                            myHousehold->move_up(firstConsumerintargetHousehold, position);
                            //myHousehold->setConsumers(myHousehold->move_up(myHousehold->getConsumers(), position));
                            break;
                        }
                    }
                    
                    break;
                    
                case 'p':
                    if (myHousehold != nullptr) {
                        house->print_household(house);
                    }
                    break;
                    
                case 'a':
                    // Print all households using the concept of traverse across the array on heap
                    house->print_household(house);
                    break;
                    
                case 'n':
                {
                    // New household
                    Household *newHousehold = new Household;
                    
                    
                    /*Z*/
                    // Check if the household with the given city already exists
                    bool cityExists = false;
                    
                    // If the city doesn't exist
                    if (!cityExists)
                    {
                        // Find an empty slot in the array of households
                        int emptySlotIndex = -1;
                        
                        
                        for (int i = 0; i < house->getNumberOfHouseholds(); ++i)
                        {
                            if (house->get_household(i) == nullptr)
                            {
                                emptySlotIndex = i;
                                //households[i]->cityName;
                                break;
                            }
                        }
                        
                        
                        // If there's an empty slot, add the new household
                        if (emptySlotIndex != -1)
                        {
                            
                            myHousehold->  input_household(newHousehold); // Call the input_household function for the new household
                            house->set_household(newHousehold,emptySlotIndex);
                        }
                        else
                        {
                            // Handle the case where there are no empty slots in the array (array is full)
                            std::cout << "Error: No space available for a new household." << std::endl;
                            delete newHousehold; // Free memory for the new household
                        }
                    }
                    
                    break;
                }
                    
                case 'c': {
                    // Input the source household number
                    int sourceHouseholdNumber;
                    std::cout << "Enter the source household number: ";
                    std::cin >> sourceHouseholdNumber;
                    
                    int sourceIndex = -1;
                    for (int i = 0; i <  house->getNumberOfHouseholds(); ++i) {
                        if (house->get_household(i) != nullptr && house->get_household(i)->getHouseNumber() == sourceHouseholdNumber) {
                            sourceIndex = i;
                            break;
                        }
                    }
                    
                    if (sourceIndex == -1) {
                        std::cout << "Error: Source household not found." << std::endl;
                        break;
                    }
                    
                    // Input the destination household number
                    int destHouseholdNumber;
                    std::cout << "Enter the destination household number: ";
                    std::cin >> destHouseholdNumber;
                    
                    int destIndex = -1;
                    for (int i = 0; i <  house->getNumberOfHouseholds(); ++i) {
                        if (house->get_household(i)!= nullptr && house->get_household(i)->getHouseNumber()  == destHouseholdNumber) {
                            destIndex = i;
                            break;
                        }
                    }
                    
                    if (destIndex == -1) {
                        std::cout << "Error: Destination household not found." << std::endl;
                        break;
                    }
                    
                    // Call the copy_consumers function with the specified source and destination households
                    
                    myHousehold-> copy_devices(house->get_household(sourceIndex), house->get_household(destIndex));
                    
                    std::cout << "Consumers copied from household " << sourceHouseholdNumber << " to household " << destHouseholdNumber << std::endl;
                    
                    break;
                }
                    
                case 'w':
                    std::cout << "Input file name: ";
                    std::cin >> fileName; // Fix the typo here
                    std::cout << "Input separator character: ";
                    std::cin >> separator;
                    house->writeDataToFile(fileName, separator, house);
                    break;
                    
                case 'r':
                    std::cout << "Enter the file name to read from: ";
                    std::cin >> fileName;
                    std::cout << "Enter the separator character: ";
                    std::cin >> separator;
                    //readDataFromFile("./" + fileName, separator, households, numHouseholds, city, pricePerKWhEUR);
                    house->readDataFromFileTest("./" + fileName,separator, house);
                    //city=parseFromFileName("./" + fileName,separator);
                    break;
                    
                case 'h':
                    // House initialization
                    if (house != nullptr)
                    {
                        std::cout << "Error: House already initialized." << std::endl;
                    }
                    else
                    {
                        // Input the number of households and address
                        int numHouseholds;
                        std::cout << "Enter the number of households in the house: ";
                        std::cin >> numHouseholds;
                        std::string street, number, zip, city;
                        std::cout << "what is the street name?: ";
                        std::cin >> street;
                        std::cout << "what is house number?";
                        std::cin >> number;
                        std::cout << "what is zip code? ";
                        std::cin >> zip;
                        std::cout << "what is the city name? ";
                        std::cin >> city;
                        
                        // Create a new House object on the heap
                        house = new House(numHouseholds, Address(street, number, zip, city));
                    }
                    break;
            }
        }
        // Clean up memory for newConsumer
        //delete newConsumer;
        return 0;
    }
    

                                   

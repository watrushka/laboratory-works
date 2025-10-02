class Hotel:
    def __init__(self, hotel_name, total_rooms_count):
        if total_rooms_count <= 0:
            raise ValueError("Total number of rooms must be positive")
        
        self.name = hotel_name
        self.total_rooms = total_rooms_count
        self.occupied_rooms = 0
        self.daily_payment = 0.0
        self.is_rate_set = False
    
    def get_name(self):
        return self.name
    
    def get_occupied_rooms(self):
        return self.occupied_rooms
    
    def get_total_rooms(self):
        return self.total_rooms
    
    def get_available_rooms(self):
        return self.total_rooms - self.occupied_rooms
    
    def get_daily_payment(self):
        return self.daily_payment
    
    def set_daily_rate(self, rate):
        if rate < 0:
            raise ValueError("Room price cannot be negative")
        self.daily_payment = rate
        self.is_rate_set = True
    
    def check_in(self, rooms=1):
        if rooms <= 0 or self.occupied_rooms + rooms > self.total_rooms:
            return False
        self.occupied_rooms += rooms
        return True
    
    def check_out(self, rooms=1):
        if rooms <= 0 or self.occupied_rooms - rooms < 0:
            return False
        self.occupied_rooms -= rooms
        return True
    
    def calculate_total_revenue(self, days):
        if days <= 0:
            raise ValueError("Number of days must be greater than zero")
        if not self.is_rate_set:
            raise RuntimeError("Price is not set")
        return self.occupied_rooms * self.daily_payment * days
    
    def display_info(self):
        print(f"Hotel: {self.name}")
        print(f"Rooms occupied: {self.occupied_rooms}/{self.total_rooms}")
        print(f"Daily rate: {self.daily_payment}")
        print(f"Available rooms: {self.get_available_rooms()}")


def main():
    try:
        hotel = Hotel("Grand Hotel", 100)
        hotel.set_daily_rate(1000)
        hotel.check_in(5)
        hotel.display_info()
        
        revenue = hotel.calculate_total_revenue(7)
        print(f"\nTotal revenue for 7 days: {revenue} USD")
        
        hotel.check_out(2)
        hotel.display_info()
        
    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    main()

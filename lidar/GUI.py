import pygame
import math


class UserInterface:

    def __init__(self):

        # Display Dimensions
        self.WIDTH = 800
        self.HEIGHT = 541

        self.MID_X = 270.5
        self.MID_Y = 270.5

        # Colors
        self.white = (255, 255, 255)
        self.black = (18, 18, 18)
        self.blue = (45, 51, 58)
        self.grey = (21, 37, 53)
        self.magenta = (255,0,230)
        self.green = (0, 255, 0)

        # Dash-board variables
        self.velocity = 0
        self.mode = "MANUAL"
        self.steering_angle = 0
        self.gas_pressure = 20
        self.dist_to_gate = 1
        self.dist_to_middle = 0
        self.lap_time = 20
        self.num_laps = 2
        self.angle_to_gate = 90
        self.previous_conepair = "START"

        self.mode_changed = False

        self.QUIT_PRESSED = False

        # Key Listeners
        self.KEY_PRESSED = "[NO_KEY_PRESSED]"

        # initialising pygame
        pygame.init()

        # creating display
        self.display = pygame.display.set_mode((self.WIDTH, self.HEIGHT))
        # set the pygame window name
        pygame.display.set_caption('GUI')
        
        # set car as rectangle and set start postion
        self.carImg = pygame.draw.rect(self.display, self.grey, 
                                       pygame.Rect(30, 30, 60, 60))

        self.font_size = 17
        self.posX, self.posY = 555, self.HEIGHT // 30
        self.font = pygame.font.Font(
            pygame.font.get_default_font(), self.font_size)

        # Render mode-button
        self.switch_mode = self.font.render("Byt läge", True, self.white)

    def change_mode(self):
        if self.mode == "AUTO":
            self.mode = "MANUAL"

        else:
            self.mode = "AUTO"

        self.mode_changed = True

    def draw_coord_system(self):
        for y in range(541):
            if y % 30 == 0:
                pygame.draw.line(self.display, self.white, [
                                 0, y], [self.posX-15, y], 2)

        for x in range(541):
            if x % 30 == 0:
                pygame.draw.line(self.display, self.white, [
                                 x, 0], [x, self.HEIGHT], 2)
                
    def draw_track(self):
        pygame.draw.circle(self.display, self.white, tuple(40, 80), 20)
        # här ska vi rita banan

    def display_dash_board(self):
        dash_board = [f"Hastighet: {self.velocity} km/h",
                      f"Läge: {self.mode}",
                      f"Avstånd nästa port: {self.dist_to_gate} cm",
                      f"Avstånd till mitten: {self.dist_to_middle} cm",
                      f"Antal varv: {self.num_laps}",
                      f"Vinkel till nästa port: {self.angle_to_gate}°",
                      f"Föregånende konpar: {self.previous_conepair}"]

        label = [self.font.render(line, True, self.white, self.black)
                 for line in dash_board]

        # display the dash-board
        for i in range(len(label)):
            self.display.blit(
                label[i], (self.posX, self.posY+(i*self.font_size)+(10*i)))

    def draw_car(self):
         pygame.draw.rect(self.display, self.magenta, 
                        pygame.Rect(self.MID_X - 30, self.HEIGHT - 60, 60, 60))
        
         pygame.draw.circle(self.display, self.black, (self.MID_X, self.HEIGHT - 30), 5)


    def update_dash_board(self, dash_board_list):
        print("Dashboardlist: ")
        print(dash_board_list)
       
        # hastighet, portdistance, avstånd till mitten, 
        # vinkel till port, varv, previous conepair"
        self.velocity = dash_board_list[0]
        #self.steering_angle = dash_board_list[1]
        #self.gas_pressure = dash_board_list[2]
        self.dist_to_gate = round(float(dash_board_list[1]) / 10)
        #self.lap_time = dash_board_list[4]
        self.num_laps = dash_board_list[4]
        self.angle_to_gate = round(float(dash_board_list[3]))
        self.dist_to_middle = round(float(dash_board_list[2]) / 10)
        self.previous_conepair = dash_board_list[5]
    
    def draw_cones(self, cone_data):
        print("conedata: ")
        print(cone_data)
        #cone: deg, dist, type
        cone1 = cone_data[0: 3]
        cone2 = cone_data[3: 6]
        cone_list = [cone1, cone2]
        for cone in cone_list:
            deg = int(cone[0])
            dist = float(cone[1])
            cone_type = cone[2]
            side = -1
            if deg > 90:
                deg = 180 - deg
                side = 1
            x_coord = self.MID_X + side * math.cos(math.radians(deg)) * dist / 5
            y_coord = self.HEIGHT - 60 - math.sin(math.radians(deg)) * dist / 5
            radius = 10
            if cone_type == "BIG":
                radius = 20
            if x_coord < 2 * self.MID_X:
                pygame.draw.circle(self.display, self.green, (x_coord, y_coord), radius)


    def GUI_loop(self, input_string):
        # input_string: "deg1, dist1, type1, 
        #                deg2, dist2, type2,
        #                hastighet, portdistance, avstånd till mitten, 
        #                vinkel till port, varv, previous cone""
        input = input_string.split(", ")

        # clear display
        self.display.fill(self.black)

        # draw the coordinate system
        self.draw_coord_system()
        self.carImg

        # draw the car
        self.draw_car()

        if len(input) >= 12:
            # update dash-board
            self.update_dash_board(input[6::])

            # draw cones
            self.draw_cones(input[0: 6])
       
        # display the dash-board
        self.display_dash_board()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.QUIT_PRESSED = True
                pygame.quit()
                # sys.exit()
                return

            # checking if keydown event happened or not
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_UP:
                    self.KEY_PRESSED = "UP"

                elif event.key == pygame.K_DOWN:
                    self.KEY_PRESSED = "DOWN"

                elif event.key == pygame.K_LEFT:
                    self.KEY_PRESSED = "LEFT"

                elif event.key == pygame.K_RIGHT:
                    self.KEY_PRESSED = "RIGHT"

                elif event.key == pygame.K_SPACE:
                    self.KEY_PRESSED = "SPACE"

            if event.type == pygame.KEYUP:
                self.KEY_PRESSED = "[NO_KEY_PRESSED]"

            # get mouse position
            mouse = pygame.mouse.get_pos()
            if event.type == pygame.MOUSEBUTTONDOWN:
                # if the mouse is clicked on the button
                if 600 <= mouse[0] <= 600+140 and self.HEIGHT/2 <= mouse[1] <= self.HEIGHT/2+40:
                    self.change_mode()

            # if mouse is hovered on a button it changes to lighter shade
            if 600 <= mouse[0] <= 600+140 and self.HEIGHT/2 <= mouse[1] <= self.HEIGHT/2+40:
                pygame.draw.rect(self.display, self.blue, [
                                 600, self.HEIGHT/2, 140, 40])

            else:
                pygame.draw.rect(self.display, self.grey, [
                                 600, self.HEIGHT/2, 140, 40])

            # superimposing the text onto our button
            self.display.blit(self.switch_mode, (600 + 36, self.HEIGHT/2 + 11))
            #pygame.display.update()
        pygame.display.update()

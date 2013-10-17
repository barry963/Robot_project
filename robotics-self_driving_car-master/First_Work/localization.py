
colors = [['red', 'green', 'green', 'red' , 'red'],
          ['red', 'red', 'green', 'red', 'red'],
          ['red', 'red', 'green', 'green', 'red'],
          ['red', 'red', 'red', 'red', 'red']]

measurements = ['green', 'green', 'green' ,'green', 'green']


motions = [[0,0],[0,1],[1,0],[1,0],[0,1]]

sensor_right = 1

p_move = 0.8

def show(p):
    for i in range(len(p)):
        print p[i]
 
#DO NOT USE IMPORT
#ENTER CODE BELOW HERE
#ANY CODE ABOVE WILL CAUSE
#HOMEWORK TO BE GRADED
#INCORRECT

p = []

times = 0
def sense(colors,measurements,times):
    global p
    green_count = 0
    red_count = 0
    for state in colors:
        for state_cell in state:
            if state_cell == 'green':
                green_count +=1
            else:
                 red_count+=1
    p = colors
    if times == 0:
        if measurements[times] == 'green':
             for state in p:
                 for state_cell in range(len(state)):
                     if state[state_cell] == 'green':
                         state[state_cell] = 1./green_count
                     else:
                         state[state_cell] = 0
                              
        else:
             for state in p:
                 for state_cell in range(len(state)):
                     if state[state_cell] == 'red':
                         state[state_cell] = 1./red_count
                     else:
                         state[state_cell] = 0
    else:
        
"""
def sense(p, Z):
    q=[]
    for i in range(len(p)):
        hit = (Z == world[i])
        q.append(p[i] * (hit * pHit + (1-hit) * pMiss))
    s = sum(q)
    for i in range(len(q)):
        q[i] = q[i] / s
    return q
"""
def move_right(times,p_move):
    global p
    moved_p = [[0]*5]*5
    for state in p:
        i = p.index(state)
        for state_cell in range(len(state)):
            if state[state_cell] != 0:
                moved_p[i][(state_cell+1)%len(state)] = p[i][(state_cell+1)%len(state)] * p_move

    
def move_left(times):

def move_up(times):

def move_down(times):

def move_none(times):

def move(p, motions,times,p_move):
"""
"""
    q = []
    for command in motions:
        {
            [0,0]:  move_none
            [0,1]:  move_right
            [0,-1]: move_left
            [1,0]:  move_down
            [-1,0]: move_up
        }[command](times,p_move)   

    return q

for k in range(len(measurements)):
    p = sense(p, measurements[k])
    p = move(p, motions[k])
    
print p  
"""


#Your probability array must be printed 
#with the following code.
sense(colors,measurements)
show(p)






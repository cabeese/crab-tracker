class Crab:
    def __init__(self, crab_id, sex, species, color, damage, carapace, mass, epibiont, molt):
        self.id = crab_id
        self.sex = sex
        self.species = species
        self.color = color
        self.damage = damage
        self.carapace = carapace
        self.mass = mass
        self.epibiont = epibiont
        self.molt = molt

    def get_tuple(self):
        return self.id, self.sex, self.species, self.color, self.damage, self.carapace, self.mass, self.epibiont, self.molt

import itertools
from hashlib import sha1

class FilesForPiece:
    def __init__(self, files = None):
        self.files = files
        self.is_valid_combination = None
        
    def add(self, new_file):
        if self.files == None:
            self.files = []
        
        self.files.append(new_file)


    # мне кажется что представленная логика немного ошибочна (тот файл который был отмечен на предыдушем шаге, как валидный
    # на текущем шаге может оказаться не валидным и мы его переотметим, но валидный файл текущего шака может дать ошибку на предыдушем
    # но наверное это редкая ситуация которая может возникнуть только при большом кол-ве кандидатов и еще при определенных условиях)
    def find_correct_file_combination(self, phash):
        files_combinations = self.make_files_combinations()
            
        for combination in files_combinations:
            self.apply_combination(combination)
            
            data = self.get_data()
            computed_hash = sha1(data).digest()
            
            self.is_valid_combination = (computed_hash == phash)
            
            if self.is_valid_combination:
                self.set_current_combination_as_matched()
                self.update_status_of_reference_files("MATCH_FOUND")
                break
            else:
                self.update_status_of_reference_files("CHECKED_WITH_NO_MATCH")


    def make_files_combinations(self):
        list_of_list_of_file = []
        for contributing_file in self.files:
            list_of_list_of_file.append(contributing_file.get_all_possible_file())
        
        cartesian_product = itertools.product(*list_of_list_of_file)
        return cartesian_product

    def apply_combination(self, combination):
        for file, contributing_file in zip(combination, self.files):
            contributing_file.possible_file = file
  
    def get_data(self):
        data = b''
        for contributing_file in self.files:
            data += contributing_file.get_data()
        return data

    def set_current_combination_as_matched(self):
        for contributing_file in self.files:
            contributing_file.apply_match_file_as_positive()
  
    def update_status_of_reference_files(self, status):
        for contributing_file in self.files:
            contributing_file.update_status(status)
